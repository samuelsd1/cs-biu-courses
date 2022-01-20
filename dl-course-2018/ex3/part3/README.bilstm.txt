TRAIN:
bilstmTrain.py repr trainFile modelFile

if you want to add your own dev, just do
bilstmTrain.py repr trainFile modelFile devFile

if you want to evaluate accuracy by NER accuracy - add '-n' to the flags.
no additional options, due to a trick i did - just zipped together some files.


TAG:
bilstmTag.py repr modelFile inputFile

no additional options :)