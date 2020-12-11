ARCH=`arch`
export PATH=$PATH:bin/$ARCH

function msg {
    if [ $SAVING ]; then
	echo >> $SAVING
	echo "# $@" >> $SAVING
	echo >> $SAVING
    else
	echo $@
    fi
}

function run {
    if [ $SAVING ]; then
	echo "run $@" >> $SAVING
    else
	echo "Execing: $@"
	(echo "$@" | sh) || { echo "Command failed: $@"; echo; exit 1; }
    fi
}

function run2 {
    if [ $SAVING ]; then
	echo "$@" >> $SAVING
    else
	echo "Execing: $@"
	echo "$@" | sh
    fi
}

function loadDataSetConfig {
    configDir=$1

    if [ $2 == "-s" ]; then
        export SAVING=scripts/`basename $0`_$3.sh
	export YES=1
	shift
	echo "#!/bin/sh" > $SAVING
	echo "PATH=\$PATH:bin/\`arch\`" >> $SAVING
	echo "export PATH" >> $SAVING
	echo "run() {" >> $SAVING
	echo "    \$@ || { echo "Command failed: \$@"; echo; exit 1; }" >> $SAVING
	echo "}" >> $SAVING
	echo "Saving all commands to $SAVING"
    fi

    if [ $2 == "-f" ]; then
	echo "Assuming yes for all questions"
        export YES=1
	shift
    fi

    export DATASET_NAME=$2
    DATASET=scripts.gen/$configDir/$2

    if [ ! -f $DATASET ]; then
	echo "Please specify a dataset as a parameter. Your choices are:" `ls -1 scripts.gen/$configDir/`
	exit 1
    fi

    source $DATASET
}

# Asks a yes/no question. The first argument is a prompt,
# the second is a default
function askYesNo {
    if [ $YES ]; then
      echo "$1 ... yes"
      true; return
    fi
    echo $1
    read YN
    (echo ${YN:=$2} | grep y > /dev/null); return
}

# The first parameter is a filename that serves as an indicator as to whether some lengthy compuatiation has already taken
# place. If this file exists, then we assume the computation results were saved and we ask the user whether he/she wants to
# redo the computation. The second paramters is an appropriately worded prompt string. This function returns true if the
# user want the computation to take place 
function ifFileExistsThenPrompt {
    if [ -r $1 ]; then
	askYesNo "$2" "n"
	return
    fi
    true
}