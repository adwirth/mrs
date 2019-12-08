# mrs
1.	Task: reconstruct and present image of an object read-in from the raw spectrometer data file

Description: The raw spectrometer data file is provided in the MRD format. The data is of a single-slice MRI scan acquired in transverse orientation on a water-filled tube placed in the scanner. The file contains a sequence of complex numbers  (‘Data’, or ‘data proper’) representing 2D data known as ‘k-space’ in MRI. The dimensions of the ‘Data’ are contained in the header of the file. The principal operation is to convert the ‘k-space’ data into ‘image-space’ data (or simply ‘image’) by means of Fourier transformation (e.g. FFT or DFT). The following steps are necessary to achieve that:
2.	
3.	1) Read in the provided raw spectrometer data file in the MRD format (see format specification below) and form a 2D array (‘k-space’) of the dimensions X (‘Dimension 1’) by Y (‘Dimension 2’). Dimensions, as well as data type, should be read in from the header. The solution should be capable of reading and processing the MRD file of any ‘Dimension 1’ and ‘Dimension 2’.
4.	
5.	The highlighted locations are those of critical importance. Only two dimensions are greater than 1 in the provided MRD file, i.e. the file contains only one 2D k-space array from a single slice acquisition.
6.	
7.	Optional step: display the magnitude of the k-space graphically 

8.	2) Perform 2D Fourier transform (FFT or DFT) of the ‘k-space’ data to obtain ‘image’. 
9.	
10.	It may be necessary to use the 2D ‘fftshift’ (MATLAB-syntax command) or equivalent before and after the FT to correctly represent the data, i.e. to un-swap the quadrants.
11.	
12.	3) Display separately the magnitude and the phase components of the ‘image’ graphically2
The object (slice through the water-filled tube) should be clearly recognisable.

Optional step: calculate the signal-to-noise ratio for the magnitude ‘image’


It is advisable to use a publicly available FFT library such as FFTW or an alternative. 
he task should be coded in C++. 



13.	
14.	
 
15.	Raw spectrometer data file format

Spectral data is stored in a file with an extension .MRD.  The file format consists of:

1)	a 256 byte header
2)	a 256 byte text description
3)	the data proper
4)	an ASCII string containing the sample information filename
5)	an ASCII copy of the parameter file (.PPR file) used

byte header
Bytes (hex)	'C' data type		Usage
0-3		Long (4 byte integer)	Dimension 1, number of data samples
4-7		Long			Dimension 2, number of views (2D data)
8-B		Long			Dimension 3, number of secondary views (3D data)
C-F		Long			Dimension 4, number of slices
...					Unspecified
12-13		Int (2 byte integer)	Data type code
...					Unspecified
98-9B		Long			Dimension 5, number of echoes
9C-9F		Long			Dimension 6, number of experiments

The data type code indicates the format of individual data elements, as follows:

	0x00	unsigned char		1 byte
	0x01	signed char		1 byte
	0x02	short			2 bytes
	0x03	int			2 bytes
	0x04	long			4 bytes
	0x05	float			4 bytes
	0x06	double			8 bytes
	0x10	bit mask indicating complex data as real, imaginary pairs of base data type

All .MRD files are at present signed char, int or floating point, either real or complex.  The default is complex floating point, data type 0x15.


256 byte text
Bytes (hex)	'C' data type		Usage
100-1FF	256 bytes ASCII	Text, 0 terminated string

This is normally unused

Data
Bytes (hex)	'C' data type			Usage
200 -->		Complex or single value	Data
		Signed char, int or float

Multi-dimensional data are stored sequentially, the index of dimension 1 (samples) varying most rapidly, followed by the other dimensions in order i.e. samples (1DFT), secondary views (3DFT), views (2DFT), slices, echoes, and experiments.

Sample file name
This is the fully qualified path name of the sample information file, as a zero terminated ASCII string, currently padded to 120 bytes.

Parameters
The contents of the .PPR parameter file.  This is ASCII text, in lines delimited by <CR> <LF> (0x0d, 0x0a).  The format is described below.

In the .MRD and .SUR file, the .PPR file proper is followed by entries indicating the instrument parameter values at the time of the acquisition.

:_ObserveTransmitGain <dB10><CR><LF>
:_ObserveReceiverGain <dB10><CR><LF>
:_ObserveReceiverAttenuation <dB10><CR><LF>
:_DecoupleTransmitGain <dB10><CR><LF>
:_ObserveModulatorOffset <DACValue><CR><LF>
:_ObserveModulatorFineOffset <DACval><CR><LF>
:_DecoupleModulatorOffset <DACval><CR><LF>
:_DecoupleModulatorFineOffset <DACval><CR><LF>

<db10> is the decimal value of the gain in units of 0.1 dB.  <DACval> is the decimal value, lying between 0 and 63 includive, written to the appropriate RF unit device.

The .MRD file is terminated by the entry

:END<CR><LF>

