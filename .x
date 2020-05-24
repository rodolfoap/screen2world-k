BINARY=app
execute(){
	./simple test/camera_matrix_1280x720.yaml test/points_matrix.yaml
	./${BINARY} test/placewilson.jpg
	echo 'Expect: (500, 482) > [43.605129, 1.447795, 0.00]'
}
build(){
	mkdir -p build;
	pushd build &> /dev/null;
	[ -f Makefile ] || cmake .. -Wdev;
	make -j$(nproc); STATUS=$?
	popd &> /dev/null;
}
case "$1" in
	"")
		[ -f ${BINARY} ] || build
		execute
	;;
	b)
		build
	;;
	e)
		vi -p app.cpp screen2worldK.cpp screen2worldK.h CMakeLists.txt
		rm -f ${BINARY}
		build;
		execute;
	;;
esac
