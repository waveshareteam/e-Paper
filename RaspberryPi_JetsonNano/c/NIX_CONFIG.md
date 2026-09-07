# NixOS Configuration for Waveshare e-Paper (RaspberryPi_JetsonNano/c)

This project now includes a Nix flake dev shell (`flake.nix` + `flake.lock`) that builds the C examples on NixOS.

## 1. Use the dev shell without copying the whole git repo

From `RaspberryPi_JetsonNano/c`, use:

```bash
nix develop path:.
```

Do not use plain `nix develop .` in this mono-repo, because Nix may treat it as a git flake rooted at `/home/.../e-Paper` and spend time copying the full repo to the store.

## 2. Build command

Inside the dev shell:

```bash
make -j4 EPD=epd5in65f
```

`flake.nix` exports `MAKEFLAGS=USELIB_RPI=USE_DEV_LIB` so default builds use the `libgpiod` backend (compatible with this environment).

## 3. Persistent NixOS system config (SPI + non-root access)

`boot.loader.raspberryPi` was removed from nixpkgs and no longer works.
Use `raspberry-pi-nix` for modern Raspberry Pi boot/device-tree configuration.

### Option A: Use the dedicated image repo (recommended)

Use `https://github.com/anicolao/nix-epaper`, which already includes:
- SPI firmware configuration
- `spidev` kernel module
- `spi`/`gpio` groups and udev rules

### Option B: Add equivalent settings to your own system flake

In your system `flake.nix`:

```nix
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    raspberry-pi-nix = {
      url = "github:nix-community/raspberry-pi-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };
}
```

In your NixOS configuration module:

```nix
{
  imports = [ raspberry-pi-nix.nixosModules.raspberry-pi ];

  # Pi 4; use "bcm2712" for Pi 5.
  raspberry-pi-nix.board = "bcm2711";

  # Enable SPI in generated config.txt.
  hardware.raspberry-pi.config.all.base-dt-params.spi = {
    enable = true;
    value = "on";
  };

  # Ensure userspace SPI char device and permissions.
  boot.kernelModules = [ "spidev" ];
  users.groups.spi = {};
  users.groups.gpio = {};
  users.users.anicolao.extraGroups = [ "spi" "gpio" ];
  services.udev.extraRules = ''
    KERNEL=="spidev*", GROUP="spi", MODE="0660"
    KERNEL=="gpiochip*", GROUP="gpio", MODE="0660"
  '';
}
```

Apply and reboot:

```bash
sudo nixos-rebuild switch
sudo reboot
```

After reboot, verify:

```bash
ls -l /dev/spidev* /dev/gpiochip*
id
```

You should see `/dev/spidev0.0` and your user in `spi` + `gpio` groups.

## 4. Why this is needed

- `sudo ./epd` previously failed with `/dev/spidev0.0: No such file or directory`.
- That indicates SPI bus nodes were not enabled by firmware/device-tree.
- `modprobe spidev` alone is temporary and does not set persistent permissions/groups.
