{ stdenvNoCC
, lib
, python3
, gnumake
, ...
}:

stdenvNoCC.mkDerivation {
  pname = "evilcc";
  version = "0.1.0";

  src = with lib.fileset; toSource {
    root = ./.;
    fileset = unions [
      ./src
      ./evilcc
      ./Makefile
    ];
  };

  buildInputs = [
    python3
    gnumake
  ];

  buildPhase = ''
    substituteInPlace ./evilcc \
      --replace-fail "#!/usr/bin/env python3" "#!${python3}/bin/python3" \
      --replace-fail "DEFAULT_SYSROOT = os.path.dirname(__file__)" "DEFAULT_SYSROOT = \"$out/share/evilcc\"" \
  '';

  installPhase = ''
    mkdir -p $out

    install -Dm755 ./evilcc $out/bin/evilcc

    mkdir -p $out/share/evilcc
    install -Dm644 ./Makefile $out/share/evilcc/Makefile
    cp -r ./src $out/share/evilcc
  '';
}
