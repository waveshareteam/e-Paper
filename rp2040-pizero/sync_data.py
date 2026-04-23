import os
import requests
import json
import re
import shutil

API_ROOT = "http://empire.145multimedia.co.uk/api"
OUTPUT_DIR = "library_data"

def get_json(url):
    print(f"Fetching {url}...")
    try:
        response = requests.get(url)
        response.raise_for_status()
        return response.json()
    except Exception as e:
        print(f"Error fetching {url}: {e}")
        return None

def main():
    if os.path.exists(OUTPUT_DIR):
        print(f"Cleaning {OUTPUT_DIR}...")
        for item in os.listdir(OUTPUT_DIR):
            item_path = os.path.join(OUTPUT_DIR, item)
            if os.path.isdir(item_path):
                shutil.rmtree(item_path)
            else:
                os.remove(item_path)
    else:
        os.makedirs(OUTPUT_DIR)
        
    # 1. Fetch Potion Sets
    sets_data = get_json(f"{API_ROOT}/potionsets")
    if not sets_data:
        return

    sets = sets_data.get("sets", [])
    
    # Save known sets list dynamically
    known_sets = [s["Name"] for s in sets]
    with open(os.path.join(OUTPUT_DIR, "known potions.txt"), "w") as f:
        for s in known_sets:
            f.write(s + "\n")
            
    for s in sets:
        set_name = s["Name"]
        set_id = s["Id"]
        description = s.get("Description", "")
        set_path = os.path.join(OUTPUT_DIR, set_name)
        
        if not os.path.exists(set_path):
            os.makedirs(set_path)
        
        # Category Description.md
        with open(os.path.join(set_path, "Description.md"), "w", encoding="utf-8") as f:
            f.write(description)
            
        # 2. Fetch Potions for this set
        potions_data = get_json(f"{API_ROOT}/potions?set_id={set_id}")
        if not potions_data:
            continue
            
        potions = potions_data.get("potions", [])
        
        for p in potions:
            potion_name = p["Name"].strip().replace("/", "_").replace(":", "_")
            form = p.get("Form", "Unknown")
            potion_path = os.path.join(set_path, potion_name)
            
            if not os.path.exists(potion_path):
                os.makedirs(potion_path)
                
            # Details.txt
            details = []
            details.append(f"Type: {form}")
            
            ingredients = p.get("Ingredients")
            if ingredients:
                ing_lines = [f"{qty} {ing}" for ing, qty in ingredients.items()]
                details.append(f"Ingredients: {'\n'.join(ing_lines)}")
            
            # RP and Mechanical Effects
            rp_effects = p.get("RpEffects")
            mec_effects = p.get("MecEffects")
            full_lore = p.get("Lore", "")
            
            lore_text = full_lore
            
            # If RpEffects is missing, try to extract from Lore as fallback
            if not rp_effects:
                rp_match = re.split(r'Roleplaying effects?:', full_lore, flags=re.IGNORECASE)
                if len(rp_match) > 1:
                    lore_text = rp_match[0].strip()
                    rp_effects = rp_match[1].strip()
                else:
                    rp_effects = "[No specific RP effects listed]"
            
            details.append(f"Roleplay Effects: {rp_effects}")
            
            if mec_effects:
                details.append(f"Mechanical Effects: {mec_effects}")
            
            with open(os.path.join(potion_path, "Details.txt"), "w", encoding="utf-8") as f:
                f.write("\n".join(details))
                
            # Body.md (Lore)
            with open(os.path.join(potion_path, "Body.md"), "w", encoding="utf-8") as f:
                f.write(lore_text.strip())

    print(f"Done! Data generated in '{OUTPUT_DIR}' folder.")

if __name__ == "__main__":
    main()
