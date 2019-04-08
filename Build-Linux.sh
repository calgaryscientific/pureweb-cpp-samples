# check for prerequisites

if [[ -z "${PUREWEB_HOME}" ]]; then
    echo "PUREWEB_HOME environment variable is not set"
    exit 1
else
	echo "PUREWEB_HOME=${PUREWEB_HOME}"
fi

if [[ -z "${PUREWEB_LIBS}" ]]; then
    echo "PUREWEB_LIBS environment variable is not set"
    exit 1
else
	echo "PUREWEB_LIBS=${PUREWEB_LIBS}"
fi

echo "Checking qmake installed"
qmake-qt5 --version &> /dev/null
if [ $? != 0 ]; then
    echo "qmake-qt5 is not installed, exiting build"
    exit 1
fi

# Deploy the Scribble HTML5 sample to Tomcat
echo "Building Scribble Qt"
cd ScribbleAppQt
if [ ! -d "debug" ]; then
    mkdir debug
fi

if [ ! -d "debug/.obj" ]; then
    mkdir debug/.obj
fi

if [ ! -d "debug/.moc" ]; then
    mkdir debug/.moc
fi

qmake-qt5 -makefile scribble_solo.pro
make
if [ ! -d "${PUREWEB_HOME}/apps" ]; then
    mkdir "${PUREWEB_HOME}/apps"
fi
if [ ! -d "${PUREWEB_HOME}/apps/ScribbleAppQt" ]; then
    mkdir "${PUREWEB_HOME}/apps/ScribbleAppQt"
fi
cp debug/scribble ${PUREWEB_HOME}/apps/ScribbleAppQt
cp ${PUREWEB_LIBS}/C++/lib/*.so ${PUREWEB_HOME}/apps/ScribbleAppQt

cd ..

echo "Samples deployed to Tomcat"
