# LM-and-GRT-for-Linux
This is a project where the incoming data from Leap Motion goes directly to its recognition with Nick Gillian's Gesture Recognition Toolkit

It loads the training data for the recognition of fingers extended and separated among each other, or extended but together. It is easy to modify in order to save the own gesture. By now, it only uses a Naive Bayes Classifier, but it may be changed to some other GRT classifier.

It requires to have the GRT library installed, as well as the LM SDK and the LM daemon. It has been tested in Linux Mint 16 Debian Based and in Linux Ubuntu 14.04 (Trusty).
