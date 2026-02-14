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

Add the following to `/etc/nixos/configuration.nix`.

```nix
{
  # Ensure groups exist for device permissions.
  users.groups.spi = {};
  users.groups.gpio = {};

  # Add your user to the groups.
  users.users.anicolao.extraGroups = [ "spi" "gpio" ];

  # Device permissions for non-root SPI/GPIO access.
  services.udev.extraRules = ''
    KERNEL=="spidev*", GROUP="spi", MODE="0660"
    KERNEL=="gpiochip*", GROUP="gpio", MODE="0660"
  '';

  # Load spidev module.
  boot.kernelModules = [ "spidev" ];

  # Raspberry Pi firmware/device-tree SPI enablement.
  boot.loader.raspberryPi = {
    enable = true;
    version = 4; # adjust for your board generation
    firmwareConfig = ''
      dtparam=spi=on
    '';
  };
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
