#!/bin/sh
PATH=$PATH:bin/`arch`
export PATH
run() {
    $@ || { echo Command failed: $@; echo; exit 1; }
}

# Creating directory for training data

run mkdir -p train/scraps/Bayesian

# Training the Bayesian classifier

run csuBayesianTrain -imDir data/csuScrapShots/normSep2002sfi imagelists/scrap_all.srt train/scraps/Bayesian/bayesian

# Creating directories for distances and/or cleaning out old distances

run mkdir -p distances/scraps/Bayesian_ML
run mkdir -p distances/scraps/Bayesian_MAP
rm distances/scraps/Bayesian_ML/*
rm distances/scraps/Bayesian_MAP/*

# Running the Bayesian classifier

run csuBayesianProject -imDir data/csuScrapShots/normSep2002sfi train/scraps/Bayesian/bayesian.intra train/scraps/Bayesian/bayesian.extra imagelists/scrap_all_x4.srt distances/scraps/Bayesian_ML distances/scraps/Bayesian_MAP
