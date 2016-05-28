# Foobar2000Filters
Foobar2000 Upsampler and IIR Filters
Uses Butherworth, Eliptic, Bessel, Chebyshev and Legendre IIR filters
There are 2 projects: foo_upsampler and foo_filtertree

# foo_upsampler
Changes input sample rate from 44.1, 48, 88.2 or 96kHz to 88.2, 96, 176.4 or 192kHz using integer convertion rates 2 or 4. Data with sample rates 176.4 or 192kHz passed through as is.
The design goal was to find the best compromise between aliasing and ringing for use in R2R NOS DAC I've built. Most of the other implementation introduces use near brick wall filters which is not really necesary and produce excessive ringing.  

# foo_filtertree
Flexible crossover and DRC using IIR filters. Can be used for up to 32 result channels. 
Following building blocks are supported: 
* Input - single or multichannles. Lomited by Foobar to 32 channels
* Output - can be used anywhere in the chain. Supports delay. 
* Filter - low pass, high pass, band pass, band stop, low/high/band shelf.
* Mixer - mixes all channels of selected Virtual Outputs(s) into one. Useed to create subwoofer channels
* Virtual Output  - used to create input for mixers
* Filer Definition - reusable chain of Filters 
* Filter Reference - Filter Definition used in signal chain

#Acknolodgments
The project uses following open sourse projects:
* Nice DSP Filters library written by Vinie Falco: https://github.com/vinniefalco/DSPFilters/tree/master/shared/DSPFilters
* Foobar2000 SDK - PC audio player of my choise for last 10+ years. 
