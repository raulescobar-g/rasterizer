
{ lib
, stdenv
, cmake
}:

stdenv.mkDerivation {
  name = "raster";

  src = lib.sourceByRegex ./. [
    "^src.*"
    "CMakeLists.txt"
  ];

  nativeBuildInputs = [ cmake ];
}
