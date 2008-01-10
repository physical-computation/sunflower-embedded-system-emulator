#!/bin/csh

foreach bunch ( $argv )
echo "Running experiment for $bunch"

#set NOVEL_GRAPH_DIR="data/bunchgraphconfig/GD_$bunch"
#set FACE_GRAPH_DIR="data/bunchgraphconfig/FG_$bunch"
set PHASE2_DIR="distances/bunchgraphconfig/Phase02_$bunch/"
set RESULTS_DIR="results/EBGM/BunchGraphConfig"
set RESULTS_FILE="results/EBGM/BunchGraphConfig/$bunch"
#set BUNCH_LIST="$bunch"

mkdir -p $RESULTS_DIR
#mkdir -p $NOVEL_GRAPH_DIR $FACE_GRAPH_DIR $PHASE2_DIR 

echo "Running permuation code for $bunch"

bin/i686/csuAnalyzePermute $PHASE2_DIR imagelists/list640.srt $RESULTS_FILE 

end
