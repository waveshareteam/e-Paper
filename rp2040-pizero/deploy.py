import os
import shutil
import re

# Mapping of directory names to device parameters
DEVICES = {
    "10.85inch_e-Paper": (1360, 960),
    "10.85inch_e-Paper_G": (1360, 960),
    "10in2_e-Paper_G": (1024, 758),
    "13.3inch_e-Paper_E": (960, 680),
    "1in54_e-Paper_G": (200, 200),
    "2.9inch_e-Paper_G": (128, 296),
    "2in13_e-Paper_G": (122, 250),
    "2in15_e-Paper_B": (160, 296),
    "2in15_e-Paper_G": (160, 296),
    "2in7_V2_e-Paper": (176, 264),
    "3.6inch_e-Paper_E": (240, 416),
    "3in52_e-Paper_B": (240, 360),
    "3in5_e-Paper_G": (184, 384),
    "3in7_e-Paper_G": (240, 416),
    "3in97_e-Paper": (480, 800),
    "3in97_e-Paper_G": (480, 800),
    "4in2_e-Paper_G": (400, 300),
    "4inch_e-Paper_E": (400, 600),
    "5in83_e-Paper_G": (648, 480),
    "5inch_e-Paper": (600, 448),
    "7in5_e-Paper_H": (800, 480),
}

SRC_DIR = "."
BASE_TARGET_DIR = "../E-paper_Separate_Program"

def deploy_to(folder):
    params = DEVICES.get(folder)
    if not params:
        return

    width, height = params
    target_path = os.path.join(BASE_TARGET_DIR, folder, "rp2040-pizero")
    
    # Remove old version if exists
    if os.path.exists(target_path):
        shutil.rmtree(target_path)
    
    os.makedirs(target_path, exist_ok=True)

    print(f"Deploying Demo to {folder} ({width}x{height})...")

    # Copy all files
    for root, dirs, files in os.walk(SRC_DIR):
        if "target" in root: continue
        if ".git" in root: continue
        if "deploy.py" in files and root == SRC_DIR:
             # Skip the deploy script itself if we don't want it in the target
             pass

        rel_path = os.path.relpath(root, SRC_DIR)
        target_root = os.path.join(target_path, rel_path)
        os.makedirs(target_root, exist_ok=True)

        for file in files:
            if file == "deploy.py" and root == SRC_DIR: continue # Skip the script itself
            
            src_file = os.path.join(root, file)
            dst_file = os.path.join(target_root, file)

            if file == "main.rs":
                with open(src_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                # Update dimensions in main.rs
                content = content.replace("Size::new(176, 264)", f"Size::new({width}, {height})")
                with open(dst_file, 'w', encoding='utf-8') as f:
                    f.write(content)
            else:
                shutil.copy2(src_file, dst_file)

if __name__ == "__main__":
    if not os.path.exists(BASE_TARGET_DIR):
        print(f"Error: {BASE_TARGET_DIR} not found. Run from rp2040-pizero directory.")
    else:
        folders = [f for f in os.listdir(BASE_TARGET_DIR) if os.path.isdir(os.path.join(BASE_TARGET_DIR, f))]
        for folder in folders:
            deploy_to(folder)
        print("Done! All models updated with the Rust Hardware Demo.")
