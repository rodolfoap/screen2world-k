BINARY=app
execute(){
	./${BINARY} test/camera_matrix_1280x720.yaml test/points_matrix.yaml
	echo -e '\nExpected: (500, 482) > [43.605129, 1.447795, 0.00]\n'
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
		vi -p app.cpp CMakeLists.txt
		rm -f ${BINARY}
		build;
		execute;
	;;
esac
