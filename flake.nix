{
  description = "evilcc";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        mkDevShell = pkgs: pkgs.mkShell {
          packages = with pkgs; [
            gcc
            gnumake
            gdb
          ];
        };
      in
      {
        devShells = {
          default = mkDevShell pkgs;

          cross_x86_64 = mkDevShell pkgs.pkgsCross.x86_64-linux;
          cross_i386 = mkDevShell pkgs.pkgsCross.i686-linux;
          cross_aarch64 = mkDevShell pkgs.pkgsCross.aarch64-multiplatform;
        };
      }
    );
}
