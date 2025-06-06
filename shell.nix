{ pkgs ? import <nixpkgs> {}
, ...
}:

pkgs.mkShell {
  packages = with pkgs; [
    gcc
    gnumake
    patchelf
    entr
  ];

  MAKE = "${pkgs.gnumake}/bin/make -j";
}
