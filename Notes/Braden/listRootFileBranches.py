# Braden Allmond, August 11th 2022, KSU

import ROOT
import sys

ROOT.gROOT.SetBatch(True) # sets visual display off (i.e. no graphs/TCanvas)

files = sys.argv # all input arguments in an array
files.pop(0) # remove first arg which is always the name of the executed script

inFile = ROOT.TFile.Open(files[0],"READ")
tree = inFile.Get("demo/vbf")

listBranches = tree.GetListOfBranches()

def makeStringListOfBranches(inputBranches):
  # takes in object of type tree.GetListOfBranches
  # returns a normal python array of strings
  # the strings are the branchnames one would see from
  # tree->Print() in a ROOT terminal
  return [str(inputBranches[i]).split(' ')[1] for i in range(len(inputBranches))]


for i in makeStringListOfBranches(listBranches):
  print(i)

