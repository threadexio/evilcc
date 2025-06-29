{
  description = "evilcc";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        mkShell = pkgs: pkgs.mkShell {
          packages =
            # Host tools
            (with pkgs.buildPackages; [
              gcc
              gnumake
              python3
              gdb
            ])
            # Target toolchain
            ++
            (with pkgs; [
              stdenv.cc
            ]);
        };

        pkgs = import nixpkgs { inherit system; };
      in
      {
        devShells = {
          default = mkShell pkgs;

          cross_x86_64 = mkShell pkgs.pkgsCross.x86_64-linux;
          cross_i386 = mkShell pkgs.pkgsCross.i686-linux;
          cross_aarch64 = mkShell pkgs.pkgsCross.aarch64-multiplatform;
        };
      }
    );
}
