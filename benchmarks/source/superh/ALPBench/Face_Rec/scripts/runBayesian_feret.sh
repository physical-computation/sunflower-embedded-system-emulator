#!/bin/sh
PATH=$PATH:bin/`arch`
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

# Creating directory for training data

run mkdir -p train/feret/Bayesian

# Training the Bayesian classifier

run csuBayesianTrain -imDir data/FERET/normSep2002sfi imagelists/feret_training.srt train/feret/Bayesian/bayesian

# Creating directories for distances and/or cleaning out old distances

run mkdir -p distances/feret/Bayesian_ML
run mkdir -p distances/feret/Bayesian_MAP
rm distances/feret/Bayesian_ML/*
rm distances/feret/Bayesian_MAP/*

# Running the Bayesian classifier

run csuBayesianProject -imDir data/FERET/normSep2002sfi train/feret/Bayesian/bayesian.intra train/feret/Bayesian/bayesian.extra imagelists/all.srt distances/feret/Bayesian_ML distances/feret/Bayesian_MAP
