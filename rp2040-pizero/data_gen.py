import os
import sys
import json
from PIL import Image, ImageOps, ImageFilter
import numpy as np
import cv2

# --- CONFIGURATION ---
_SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(_SCRIPT_DIR, "library_data")
GALLERY_DIR = os.path.join(_SCRIPT_DIR, "gallery")
DATA_RS = os.path.join(_SCRIPT_DIR, "src", "data.rs")
ASSETS_RS = os.path.join(_SCRIPT_DIR, "src", "assets.rs")

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

def escape_rust(s):
    return s.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n')

# --- IMAGE PROCESSING (from convert_4gray.py) ---
def process_one_image(img_path):
    print(f"  Processing image: {os.path.basename(img_path)}...")
    gamma = 1.3
    
    with Image.open(img_path) as img:
        if img.mode == 'RGBA':
            background = Image.new("RGB", img.size, (255, 255, 255))
            background.paste(img, mask=img.split()[3])
            img = background.convert('L')
        else:
            img = img.convert('L')
        # Scale to fit within 256x168 while preserving aspect ratio
        img.thumbnail((256, 168), Image.Resampling.LANCZOS)
        
        # Center on a 264x176 white background
        img_boxed = Image.new('L', (264, 176), 255)
        offset = ((264 - img.width) // 2, (176 - img.height) // 2)
        img_boxed.paste(img, offset)
        img = img_boxed
        
        img_np = np.array(img)
        img_np = (np.power(img_np / 255.0, 1.0 / gamma) * 255.0).astype(np.uint8)
        clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
        img_np = clahe.apply(img_np)
        
        img = Image.fromarray(img_np)
        img = ImageOps.autocontrast(img, cutoff=0.3)
        img = img.filter(ImageFilter.UnsharpMask(radius=1.5, percent=180, threshold=2))
        img = img.rotate(-90, expand=True) # 176x264 Physical
        
        width, height = img.size
        pixels = np.pad(np.array(img, dtype=float), 2, mode='constant', constant_values=255)
        
        for y in range(2, height + 2):
            for x in range(2, width + 2):
                old_p = pixels[y, x]
                new_p = get_closest_level(old_p)
                pixels[y, x] = new_p
                err = (old_p - new_p) / 8.0
                pixels[y,   x+1] += err
                pixels[y,   x+2] += err
                pixels[y+1, x-1] += err
                pixels[y+1, x]   += err
                pixels[y+1, x+1] += err
                pixels[y+2, x]   += err

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

def format_hex(data):
    lines = []
    for i in range(0, len(data), 12):
        chunk = data[i:i+12]
        lines.append("        " + ",".join([f"0x{b:02X}" for b in chunk]) + ",")
    return "\n".join(lines)

# --- TEXT PROCESSING ---
def read_file(path, escape=True):
    if not os.path.exists(path): return ""
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        content = f.read().strip()
        return escape_rust(content) if escape else content

def parse_details(path):
    if not os.path.exists(path): return []
    content = read_file(path, escape=False)
    details = []
    current_label = None
    current_value = []
    for line in content.split('\n'):
        if ':' in line:
            if current_label: details.append((current_label, '\n'.join(current_value).strip()))
            label, value = line.split(':', 1)
            current_label = label.strip()
            current_value = [value.strip()]
        elif current_label and line.strip():
            current_value.append(line.strip())
    if current_label: details.append((current_label, '\n'.join(current_value).strip()))
    return details

def main():
    print("--- E-Paper Offline Data Generator ---")
    
    # 1. Process Gallery Images -> src/assets.rs
    valid_exts = (".png", ".jpg", ".jpeg", ".bmp", ".webp")
    if os.path.exists(GALLERY_DIR):
        images = sorted([f for f in os.listdir(GALLERY_DIR) if f.lower().endswith(valid_exts)])
    else:
        images = []
        
    print(f"Found {len(images)} images in gallery.")
    all_image_data = []
    for img_name in images:
        b1, b0 = process_one_image(os.path.join(GALLERY_DIR, img_name))
        all_image_data.append((img_name, b1, b0))

    with open(ASSETS_RS, "w", encoding="utf-8") as f:
        f.write("// AUTO-GENERATED GALLERY ASSETS. DO NOT EDIT.\n\n")
        for i, (name, b1, b0) in enumerate(all_image_data):
            f.write(f"static G{i}_B1: [u8; 5808] = [\n{format_hex(b1)}\n];\n\n")
            f.write(f"static G{i}_B0: [u8; 5808] = [\n{format_hex(b0)}\n];\n\n")
        f.write(f"pub const COVER_COUNT: usize = {len(all_image_data)};\n\n")
        f.write("pub static COVERS: &[(&[u8; 5808], &[u8; 5808])] = &[\n")
        for i in range(len(all_image_data)):
            f.write(f"    (&G{i}_B1, &G{i}_B0),\n")
        f.write("];\n")

    # 2. Process Documentation -> src/data.rs
    categories = []
    if os.path.exists(DATA_DIR):
        for cat_name in sorted(os.listdir(DATA_DIR)):
            cat_path = os.path.join(DATA_DIR, cat_name)
            if not os.path.isdir(cat_path) or cat_name.startswith("."): continue
            cat_data = {
                "name": cat_name,
                "description": read_file(os.path.join(cat_path, "Description.md")),
                "items": []
            }
            for item_name in sorted(os.listdir(cat_path)):
                item_path = os.path.join(cat_path, item_name)
                if not os.path.isdir(item_path): continue
                cat_data["items"].append({
                    "name": item_name,
                    "details": parse_details(os.path.join(item_path, "Details.txt")),
                    "body": read_file(os.path.join(item_path, "Body.md")),
                })
            categories.append(cat_data)

    with open(DATA_RS, "w", encoding="utf-8") as f:
        f.write("// Generated by data_gen.py. DO NOT EDIT.\n\n")
        f.write("pub struct Detail {\n    pub label: &'static str,\n    pub value: &'static str,\n}\n\n")
        f.write("pub struct Item {\n    pub name: &'static str,\n    pub details: &'static [Detail],\n    pub body: &'static str,\n}\n\n")
        f.write("pub struct Category {\n    pub name: &'static str,\n    pub description: &'static str,\n    pub items: &'static [Item],\n}\n\n")
        
        for i, cat in enumerate(categories):
            for j, item in enumerate(cat["items"]):
                f.write(f"const DETAILS_{i}_{j}: &[Detail] = &[\n")
                for label, value in item["details"]:
                    f.write(f'    Detail {{ label: "{escape_rust(label)}", value: "{escape_rust(value)}" }},\n')
                f.write("];\n\n")
            f.write(f"const ITEMS_{i}: &[Item] = &[\n")
            for j, item in enumerate(cat["items"]):
                f.write(f'    Item {{\n        name: "{escape_rust(item["name"])}",\n        details: DETAILS_{i}_{j},\n        body: "{item["body"]}",\n    }},\n')
            f.write("];\n\n")
        
        f.write("pub static DATA: &[Category] = &[\n")
        for i, cat in enumerate(categories):
            f.write(f'    Category {{\n        name: "{escape_rust(cat["name"])}",\n        description: "{cat["description"]}",\n        items: ITEMS_{i},\n    }},\n')
        f.write("];\n")

    print(f"Done! Generated {ASSETS_RS} and {DATA_RS}.")

if __name__ == "__main__":
    main()
