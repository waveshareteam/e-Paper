{
  description = "Waveshare e-Paper C dev shell for NixOS";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            bun
            gcc
            gnumake
            pkg-config
            linuxHeaders
            libgpiod_1
            wiringpi
          ];

          shellHook = ''
            export MAKEFLAGS="USELIB_RPI=USE_DEV_LIB"
            echo "Waveshare e-Paper C dev shell"
            echo "Build example: make clean && make -j4 EPD=epd2in13V4"
          '';
        };
      });
}
