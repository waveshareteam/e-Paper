"""
Apothecary Tome - E-Paper GALLERY Converter (SSD1680 4-Level Grayscale)
Version: Sequential Gallery Edition

This tool scans the 'gallery/' directory and bundles all images into the tome.

PERFECT FIT DIMENSIONS:
- Logical Orientation: Landscape (3:2 orientation, 264x176)
- Target: 16MB Flash Mode

Usage:
    python convert_4gray.py [gallery_dir] [output_rs_path]
"""

import sys
from PIL import Image, ImageOps, ImageFilter
import os
import numpy as np

import cv2

HARDWARE_MAP = {
    0:   (1, 1), # Black
    85:  (0, 1), # Dark Gray
    170: (1, 0), # Light Gray
    255: (0, 0)  # White
}

def get_closest_level(pixel):
    if pixel < 42:    return 0
    if pixel < 128:   return 85
    if pixel < 212:   return 170
    return 255

def process_one_image(img_path):
    print(f"  Processing {img_path}...")
    gamma = 1.3 # Lift mid-tones for e-paper ink curves
    
    with Image.open(img_path) as img:
        img = img.convert('L')
        
        # Phase 1: High-Fidelity Fit with Matte Border
        # Resize to 256x168 and pad to 264x176 (4px pure white border)
        img = ImageOps.fit(img, (256, 168), Image.Resampling.LANCZOS)
        img_boxed = Image.new('L', (264, 176), 255) # Pure white background
        img_boxed.paste(img, (4, 4))
        img = img_boxed
        
        # Phase 2: Gamma Correction & Local Adaptive Normalization (CLAHE)
        img_np = np.array(img)
        # Apply Gamma
        img_np = (np.power(img_np / 255.0, 1.0 / gamma) * 255.0).astype(np.uint8)
        
        clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
        img_np = clahe.apply(img_np)
        
        # Phase 3: Contrast Sharpness
        img = Image.fromarray(img_np)
        img = ImageOps.autocontrast(img, cutoff=0.3)
        img = img.filter(ImageFilter.UnsharpMask(radius=1.5, percent=180, threshold=2))
        
        # Orientation for Physical Display
        img = img.rotate(-90, expand=True) # 176x264 Physical
        
        width, height = img.size
        # High-stability padding for Atkinson kernel
        # Kernel needs: (x+2, y), (y+2, x), (x-1, y+1)
        pixels = np.pad(np.array(img, dtype=float), 2, mode='constant', constant_values=255)
        
        # Atkinson Dithering Loop (1/8 error to 6 neighbors)
        for y in range(2, height + 2):
            for x in range(2, width + 2):
                old_p = pixels[y, x]
                new_p = get_closest_level(old_p)
                pixels[y, x] = new_p
                err = (old_p - new_p) / 8.0
                
                # Spread error to the 6 neighbors
                pixels[y,   x+1] += err
                pixels[y,   x+2] += err
                pixels[y+1, x-1] += err
                pixels[y+1, x]   += err
                pixels[y+1, x+1] += err
                pixels[y+2, x]   += err

        # Extract core pixels back to physical dimensions
        pixels = np.clip(pixels[2:height+2, 2:width+2], 0, 255).astype(np.uint8)
        b1 = bytearray([0] * 5808)
        b0 = bytearray([0] * 5808)
        
        for y in range(height):
            for x in range(width):
                val = pixels[y, x]
                level = get_closest_level(val)
                ram1, ram2 = HARDWARE_MAP[level]
                byte_idx = (y * width + x) // 8
                bit_idx = 7 - (x % 8)
                if ram1: b1[byte_idx] |= (1 << bit_idx)
                if ram2: b0[byte_idx] |= (1 << bit_idx)
    return b1, b0

def main(gallery_dir="gallery", output_rs="src/assets.rs"):
    if not os.path.exists(gallery_dir):
        print(f"Creating gallery directory: {gallery_dir}")
        os.makedirs(gallery_dir)
        print("Please add images to the 'gallery/' folder and run again.")
        return

    valid_exts = (".png", ".jpg", ".jpeg", ".bmp", ".webp")
    images = [f for f in os.listdir(gallery_dir) if f.lower().endswith(valid_exts)]
    
    if not images:
        print(f"No images found in {gallery_dir}.")
        return

    print(f"Found {len(images)} images in gallery.")
    
    all_data = []
    for img_name in images:
        b1, b0 = process_one_image(os.path.join(gallery_dir, img_name))
        all_data.append((img_name, b1, b0))

    def format_hex(data):
        lines = []
        for i in range(0, len(data), 12):
            chunk = data[i:i+12]
            lines.append("        " + ",".join([f"0x{b:02X}" for b in chunk]) + ",")
        return "\n".join(lines)

    print(f"Generating {output_rs}...")
    with open(output_rs, "w") as f:
        f.write("// AUTO-GENERATED GALLERY ASSETS\n\n")
        
        # Individual buffers
        for i, (name, b1, b0) in enumerate(all_data):
            safe_name = "".join(x for x in name if x.isalnum()).upper()
            f.write(f"static G{i}_B1: [u8; 5808] = [\n{format_hex(b1)}\n];\n\n")
            f.write(f"static G{i}_B0: [u8; 5808] = [\n{format_hex(b0)}\n];\n\n")

        # Slice of slices
        f.write(f"pub const COVER_COUNT: usize = {len(all_data)};\n\n")
        f.write("pub static COVERS: &[(&[u8; 5808], &[u8; 5808])] = &[\n")
        for i in range(len(all_data)):
            f.write(f"    (&G{i}_B1, &G{i}_B0),\n")
        f.write("];\n")

    print(f"Gallery ready with {len(images)} images. Run 'cargo run --release'.")

if __name__ == "__main__":
    g_dir = sys.argv[1] if len(sys.argv) > 1 else "gallery"
    o_rs = sys.argv[2] if len(sys.argv) > 2 else "src/assets.rs"
    main(g_dir, o_rs)
