#!/usr/bin/env python

channels = [
### 11g

1,	2412e6, 	"11g",
2,	2417e6, 	"11g",
3,	2422e6, 	"11g",
4,	2427e6, 	"11g",
5,	2432e6, 	"11g",
6,	2437e6, 	"11g",
7,	2442e6, 	"11g",
8,	2447e6, 	"11g",
9,	2452e6, 	"11g",
10,	2457e6, 	"11g",
11,	2462e6, 	"11g",
12,	2467e6, 	"11g",
13,	2472e6, 	"11g",
14,	2484e6, 	"11g",

### 11a

34,	5170e6, 	"11a",
36,	5180e6, 	"11a",
38,	5190e6, 	"11a",
40,	5200e6, 	"11a",
42,	5210e6, 	"11a",
44,	5220e6, 	"11a",
46,	5230e6, 	"11a",
48,	5240e6, 	"11a",

50,	5250e6, 	"11a",
52,	5260e6, 	"11a",
54,	5270e6, 	"11a",
56,	5280e6, 	"11a",
58,	5290e6, 	"11a",
60,	5300e6, 	"11a",
62,	5310e6, 	"11a",
64,	5320e6, 	"11a",

100,	5500e6, 	"11a",
102,	5510e6, 	"11a",
104,	5520e6, 	"11a",
106,	5530e6, 	"11a",
108,	5540e6, 	"11a",
110,	5550e6, 	"11a",
112,	5560e6, 	"11a",
114,	5570e6, 	"11a",
116,	5580e6, 	"11a",
118,	5590e6, 	"11a",
120,	5600e6, 	"11a",
122,	5610e6, 	"11a",
124,	5620e6, 	"11a",
126,	5630e6, 	"11a",
128,	5640e6, 	"11a",
132,	5660e6, 	"11a",
134,	5670e6, 	"11a",
136,	5680e6, 	"11a",
138,	5690e6, 	"11a",
140,	5700e6, 	"11a",
142,	5710e6, 	"11a",
144,	5720e6, 	"11a",

149,	5745e6, 	"11a (SRD)",
151,	5755e6, 	"11a (SRD)",
153,	5765e6, 	"11a (SRD)",
155,	5775e6, 	"11a (SRD)",
157,	5785e6, 	"11a (SRD)",
159,	5795e6, 	"11a (SRD)",
161,	5805e6, 	"11a (SRD)",
165,	5825e6, 	"11a (SRD)",


### 11p
172,	5860e6, 	"11p",
174,	5870e6, 	"11p",
176,	5880e6, 	"11p",
178,	5890e6, 	"11p",
180,	5900e6, 	"11p",
182,	5910e6, 	"11p",
184,	5920e6, 	"11p"
]

labels = []
freqs  = []
for i in range(0, len(channels), 3):
	labels.append(repr(channels[i]).rjust(3) + " | " + repr(channels[i+1]/1e6) + " | " + channels[i+2])
	freqs.append(channels[i+1])


print(labels)
print(freqs)
