#!/bin/sh
cd poly_n; rm -f array.n.h; ln -s ../array/array.n.h array.n.h; cd ..
cd spherical; rm -f array.n.h; ln -s ../array/array.n.h array.n.h; cd ..
cd spherical; rm -f array.scp.h; ln -s ../array/array.scp.h array.scp.h; cd ..
cd spherical; rm -f poly.n.h; ln -s ../poly_n/poly.n.h poly.n.h; cd ..
cd pseudo; rm -f mx.slm.txt; ln -s ../omega/vector.slm.32.src mx.slm.txt; cd ..
