# Foobar2000Filters
Foobar2000 upsampler and IIR filters

Uses Butterworth, Elliptic, Bessel, Chebyshev and Legendre IIR filters
There are 2 projects: foo_upsampler and foo_filtertree

# foo_upsampler
Changes input sample rate from 44.1, 48, 88.2 or 96kHz to 88.2, 96, 176.4 or 192kHz using integer convertion rates 2 or 4. Data with sample rates 176.4 or 192kHz passed through as is.
The design goal is to find the best compromise between aliasing and ringing using R2R NOS DAC I've built. Most of the "standard" upsampling implementations use near brick wall filter which results in heavy ringing. More gently slopes are usually ok unless you care about out of band (and hearing) measuerements.  

# foo_filtertree

![logo]

[logo]: https://raw.githubusercontent.com/ViktorStolbovoy/Foobar2000Filters/master/filter_tree.JPG "Setup"

Flexible crossover and DRC that uses IIR filters. Supports up to 32 output channels (Foobar limitation). 
Following building blocks are supported: 
* Input - single or multichannel. Number of channels limited by Foobar to 32.
* Output - can be used anywhere in the chain. Supports delay. 
* Filter - low pass, high pass, band pass, band stop, low/high/band shelf.
* Mixer - mixes all channels of selected Virtual Output(s) into one. It can be used to create subwoofer channels
* Virtual Output  - used to create input for mixers
* Filer Definition - reusable chain of Filters 
* Filter Reference - Filter Definition used in the signal chain

#Acknowledgments
The project uses the following open source projects:
* Nice DSP Filters library written by Vinie Falco: https://github.com/vinniefalco/DSPFilters/tree/master/shared/DSPFilters
* Foobar2000 SDK - PC audio player of my choice for the last 10+ years. 


