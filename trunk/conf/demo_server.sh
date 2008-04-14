#!/bin/sh
# -----------------------------------------------------------------------------
#   JAVA_HOME       Must point at your Java Development Kit installation.
#   DS_HOME
#

DS_HOME=`cd ..;pwd`
CLASSPATH='.'
_RUNJAVA="$JAVA_HOME"/bin/java

if [ -e config ]; then
	CONFIG=`cat config`
fi

echo DS_HOME="$DS_HOME"
echo CONFIG="$CONFIG"
echo JAVA_HOME="$JAVA_HOME"

for i in `ls ../lib/*.jar`
do
	CLASSPATH="${CLASSPATH}:"$i;
done

echo CLASSPATH="$CLASSPATH"

if expr "$CONFIG" : 'http.*' > /dev/null; then
	CONFIG="-Dconfig=$CONFIG"
else 
	CONFIG=''
fi

if [ "$1" = "run" ]; then
	exec	"$_RUNJAVA" $CONFIG 	\
		-classpath "$CLASSPATH" \
		com.sigma.demo.ServerLauncher start 

else
	exec	"$_RUNJAVA" $CONFIG 	\
		-classpath "$CLASSPATH" \
		com.sigma.demo.ServerLauncher $1 \
		> ../logs/demoserver.out 2>&1 &
fi
