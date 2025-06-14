{ pkgs ? import <nixpkgs> {}
, ...
}:

pkgs.mkShell {
  packages = with pkgs; [
    gcc_multi
    gnumake
    patchelf
    entr
  ];
}
