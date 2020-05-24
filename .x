BINARY=locator
execute(){
	./simple test/camera_matrix_1280x720.yaml test/points_matrix.yaml
	echo 'Expect: (500, 482) > [43.605129, 1.447795, 0.00]'

	./${BINARY} test/camera_matrix_1280x720.yaml test/points_matrix.yaml test/placewilson.jpg

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
		vi -p locator.cpp screen2worldK.cpp screen2worldK.h CMakeLists.txt
		rm -f ${BINARY}
		build;
		execute;
	;;
esac
