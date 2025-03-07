QRCode
======

A simple library for generating [QR codes](https://en.wikipedia.org/wiki/QR_code) in C,
optimized for processing and memory constrained systems.

**Features:**

- Stack-based (no heap necessary; but you can use heap if you want)
- Low-memory foot print (relatively)
- Compile-time stripping of unecessary logic and constants
- MIT License; do with this as you please


Installing
----------

To install this library, download and save it to your Arduino libraries directory.

Rename the directory to QRCode (if downloaded from GitHub, the filename may be
qrcode-master; library names may not contain the hyphen, so it must be renamed)


API
---

**Generate a QR Code**

// The structure to manage the QR code
QRCode qrcode;

// Allocate a chunk of memory to store the QR code
uint8_t qrcodeBytes[qrcode_getBufferSize()];

qrcode_initText(&qrcode, qrcodeBytes, 3, ECC_LOW, "HELLO WORLD");

**Draw a QR Code**

How a QR code is used will vary greatly from project to project. For example:

- Display on an OLED screen (128x64 nicely supports 2 side-by-side version 3 QR codes)
- Print as a bitmap on a thermal printer
- Store as a BMP (or with a some extra work, possibly a PNG) on an SD card

The following example prints a QR code to the Serial Monitor (it likely will
not be scannable, but is just for demonstration purposes).

for (uint8 y = 0; y < qrcode.size; y++) {
    for (uint8 x = 0; x < qrcode.size; x++) {
        if (qrcode_getModule(&qrcode, x, y) {
            Serial.print("**");
        } else {
            Serial.print("  ");
        }
    }
    Serial.print("\n");


What is Version, Error Correction and Mode?
-------------------------------------------

A QR code is composed of many little squares, called **modules**, which represent
encoded data, with additional error correction (allowing partially damaged QR
codes to still be read).

The **version** of a QR code is a number between 1 and 40 (inclusive), which indicates
the size of the QR code. The width and height of a QR code are always equal (it is
square) and are equal to `4 * version + 17`.

The level of **error correction** is a number between 0 and 3 (inclusive), or can be
one of the symbolic names ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH. Higher
levels of error correction sacrifice data capacity, but allow a larger portion of
the QR code to be damaged or unreadable.

The **mode** of a QR code is determined by the data being encoded. Each mode is encoded
internally using a compact representation, so lower modes can contain more data.

- **NUMERIC:** numbers (`0-9`)
- **ALPHANUMERIC:** uppercase letters (`A-Z`), numbers (`0-9`), the space (` `), dollar sign (`$`), percent sign (`%`), asterisk (`*`), plus (`+`), minus (`-`), decimal point (`.`), slash (`/`) and colon (`:`).
- **BYTE:** any character


Data Capacities
---------------

<table>
    <tr>
        <th rowspan="2">Version</th>
        <th rowspan="2">Size</th>
        <th rowspan="2">Error Correction</th>
        <th colspan="3">Mode</th>
    </tr>
    <tr>
        <th>Numeric</th>
        <th>Alphanumeric</th>
        <th>Byte</th>
    </tr>
    <tr>
        <td rowspan="4">1</td>
        <td rowspan="4">21 x 21</td>
        <td>LOW</td><td>41</td><td>25</td><td>17</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>34</td><td>20</td><td>14</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>27</td><td>16</td><td>11</td>
    </tr>
    <tr>
        <td>HIGH</td><td>17</td><td>10</td><td>7</td>
    </tr>
    <tr>
        <td rowspan="4">2</td>
        <td rowspan="4">25 x 25</td>
        <td>LOW</td><td>77</td><td>47</td><td>32</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>63</td><td>38</td><td>26</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>48</td><td>29</td><td>20</td>
    </tr>
    <tr>
        <td>HIGH</td><td>34</td><td>20</td><td>14</td>
    </tr>
    <tr>
        <td rowspan="4">3</td>
        <td rowspan="4">29 x 29</td>
        <td>LOW</td><td>127</td><td>77</td><td>53</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>101</td><td>61</td><td>42</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>77</td><td>47</td><td>32</td>
    </tr>
    <tr>
        <td>HIGH</td><td>58</td><td>35</td><td>24</td>
    </tr>
    <tr>
        <td rowspan="4">4</td>
        <td rowspan="4">33 x 33</td>
        <td>LOW</td><td>187</td><td>114</td><td>78</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>149</td><td>90</td><td>62</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>111</td><td>67</td><td>46</td>
    </tr>
    <tr>
        <td>HIGH</td><td>82</td><td>50</td><td>34</td>
    </tr>
    <tr>
        <td rowspan="4">5</td>
        <td rowspan="4">37 x 37</td>
        <td>LOW</td><td>255</td><td>154</td><td>106</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>202</td><td>122</td><td>84</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>144</td><td>87</td><td>60</td>
    </tr>
    <tr>
        <td>HIGH</td><td>106</td><td>64</td><td>44</td>
    </tr>
    <tr>
        <td rowspan="4">6</td>
        <td rowspan="4">41 x 41</td>
        <td>LOW</td><td>322</td><td>195</td><td>134</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>255</td><td>154</td><td>106</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>178</td><td>108</td><td>74</td>
    </tr>
    <tr>
        <td>HIGH</td><td>139</td><td>84</td><td>58</td>
    </tr>
    <tr>
        <td rowspan="4">7</td>
        <td rowspan="4">45 x 45</td>
        <td>LOW</td><td>370</td><td>224</td><td>154</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>293</td><td>178</td><td>122</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>207</td><td>125</td><td>86</td>
    </tr>
    <tr>
        <td>HIGH</td><td>154</td><td>93</td><td>64</td>
    </tr>
    <tr>
        <td rowspan="4">8</td>
        <td rowspan="4">49 x 49</td>
        <td>LOW</td><td>461</td><td>279</td><td>192</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>365</td><td>221</td><td>152</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>259</td><td>157</td><td>108</td>
    </tr>
    <tr>
        <td>HIGH</td><td>202</td><td>122</td><td>84</td>
    </tr>
    <tr>
        <td rowspan="4">9</td>
        <td rowspan="4">53 x 53</td>
        <td>LOW</td><td>552</td><td>335</td><td>230</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>432</td><td>262</td><td>180</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>312</td><td>189</td><td>130</td>
    </tr>
    <tr>
        <td>HIGH</td><td>235</td><td>143</td><td>98</td>
    </tr>
    <tr>
        <td rowspan="4">10</td>
        <td rowspan="4">57 x 57</td>
        <td>LOW</td><td>652</td><td>395</td><td>271</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>513</td><td>311</td><td>213</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>364</td><td>221</td><td>151</td>
    </tr>
    <tr>
        <td>HIGH</td><td>288</td><td>174</td><td>119</td>
    </tr>
    <tr>
        <td rowspan="4">11</td>
        <td rowspan="4">61 x 61</td>
        <td>LOW</td><td>772</td><td>468</td><td>321</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>604</td><td>366</td><td>251</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>427</td><td>259</td><td>177</td>
    </tr>
    <tr>
        <td>HIGH</td><td>331</td><td>200</td><td>137</td>
    </tr>
    <tr>
        <td rowspan="4">12</td>
        <td rowspan="4">65 x 65</td>
        <td>LOW</td><td>883</td><td>535</td><td>367</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>691</td><td>419</td><td>287</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>489</td><td>296</td><td>203</td>
    </tr>
    <tr>
        <td>HIGH</td><td>374</td><td>227</td><td>155</td>
    </tr>
    <tr>
        <td rowspan="4">13</td>
        <td rowspan="4">69 x 69</td>
        <td>LOW</td><td>1022</td><td>619</td><td>425</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>796</td><td>483</td><td>331</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>580</td><td>352</td><td>241</td>
    </tr>
    <tr>
        <td>HIGH</td><td>427</td><td>259</td><td>177</td>
    </tr>
    <tr>
        <td rowspan="4">14</td>
        <td rowspan="4">73 x 73</td>
        <td>LOW</td><td>1101</td><td>667</td><td>458</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>871</td><td>528</td><td>362</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>621</td><td>376</td><td>258</td>
    </tr>
    <tr>
        <td>HIGH</td><td>468</td><td>283</td><td>194</td>
    </tr>
    <tr>
        <td rowspan="4">15</td>
        <td rowspan="4">77 x 77</td>
        <td>LOW</td><td>1250</td><td>758</td><td>520</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>991</td><td>600</td><td>412</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>703</td><td>426</td><td>292</td>
    </tr>
    <tr>
        <td>HIGH</td><td>530</td><td>321</td><td>220</td>
    </tr>
    <tr>
        <td rowspan="4">16</td>
        <td rowspan="4">81 x 81</td>
        <td>LOW</td><td>1408</td><td>854</td><td>586</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>1082</td><td>656</td><td>450</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>775</td><td>470</td><td>322</td>
    </tr>
    <tr>
        <td>HIGH</td><td>602</td><td>365</td><td>250</td>
    </tr>
    <tr>
        <td rowspan="4">17</td>
        <td rowspan="4">85 x 85</td>
        <td>LOW</td><td>1548</td><td>938</td><td>644</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>1212</td><td>734</td><td>504</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>876</td><td>531</td><td>364</td>
    </tr>
    <tr>
        <td>HIGH</td><td>674</td><td>408</td><td>280</td>
    </tr>
    <tr>
        <td rowspan="4">18</td>
        <td rowspan="4">89 x 89</td>
        <td>LOW</td><td>1725</td><td>1046</td><td>718</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>1346</td><td>816</td><td>560</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>948</td><td>574</td><td>394</td>
    </tr>
    <tr>
        <td>HIGH</td><td>746</td><td>452</td><td>310</td>
    </tr>
    <tr>
        <td rowspan="4">19</td>
        <td rowspan="4">93 x 93</td>
        <td>LOW</td><td>1903</td><td>1153</td><td>792</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>1500</td><td>909</td><td>624</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1063</td><td>644</td><td>442</td>
    </tr>
    <tr>
        <td>HIGH</td><td>813</td><td>493</td><td>338</td>
    </tr>
    <tr>
        <td rowspan="4">20</td>
        <td rowspan="4">97 x 97</td>
        <td>LOW</td><td>2061</td><td>1249</td><td>858</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>1600</td><td>970</td><td>666</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1159</td><td>702</td><td>482</td>
    </tr>
    <tr>
        <td>HIGH</td><td>919</td><td>557</td><td>382</td>
    </tr>
    <tr>
        <td rowspan="4">21</td>
        <td rowspan="4">101 x 101</td>
        <td>LOW</td><td>2232</td><td>1352</td><td>929</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>1708</td><td>1035</td><td>711</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1224</td><td>742</td><td>509</td>
    </tr>
    <tr>
        <td>HIGH</td><td>969</td><td>587</td><td>403</td>
    </tr>
    <tr>
        <td rowspan="4">22</td>
        <td rowspan="4">105 x 105</td>
        <td>LOW</td><td>2409</td><td>1460</td><td>1003</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>1872</td><td>1134</td><td>779</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1358</td><td>823</td><td>565</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1056</td><td>640</td><td>439</td>
    </tr>
    <tr>
        <td rowspan="4">23</td>
        <td rowspan="4">109 x 109</td>
        <td>LOW</td><td>2620</td><td>1588</td><td>1091</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>2059</td><td>1248</td><td>857</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1468</td><td>890</td><td>611</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1108</td><td>672</td><td>461</td>
    </tr>
    <tr>
        <td rowspan="4">24</td>
        <td rowspan="4">113 x 113</td>
        <td>LOW</td><td>2812</td><td>1704</td><td>1171</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>2188</td><td>1326</td><td>911</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1588</td><td>963</td><td>661</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1228</td><td>744</td><td>511</td>
    </tr>
    <tr>
        <td rowspan="4">25</td>
        <td rowspan="4">117 x 117</td>
        <td>LOW</td><td>3057</td><td>1853</td><td>1273</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>2395</td><td>1451</td><td>997</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1718</td><td>1041</td><td>715</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1286</td><td>779</td><td>535</td>
    </tr>
    <tr>
        <td rowspan="4">26</td>
        <td rowspan="4">121 x 121</td>
        <td>LOW</td><td>3283</td><td>1990</td><td>1367</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>2544</td><td>1542</td><td>1059</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1804</td><td>1094</td><td>751</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1425</td><td>864</td><td>593</td>
    </tr>
    <tr>
        <td rowspan="4">27</td>
        <td rowspan="4">125 x 125</td>
        <td>LOW</td><td>3517</td><td>2132</td><td>1465</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>2701</td><td>1637</td><td>1125</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>1933</td><td>1172</td><td>805</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1501</td><td>910</td><td>625</td>
    </tr>
    <tr>
        <td rowspan="4">28</td>
        <td rowspan="4">129 x 129</td>
        <td>LOW</td><td>3669</td><td>2223</td><td>1528</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>2857</td><td>1732</td><td>1190</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>2085</td><td>1263</td><td>868</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1581</td><td>958</td><td>658</td>
    </tr>
    <tr>
        <td rowspan="4">29</td>
        <td rowspan="4">133 x 133</td>
        <td>LOW</td><td>3909</td><td>2369</td><td>1628</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>3035</td><td>1839</td><td>1264</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>2181</td><td>1322</td><td>908</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1677</td><td>1016</td><td>698</td>
    </tr>
    <tr>
        <td rowspan="4">30</td>
        <td rowspan="4">137 x 137</td>
        <td>LOW</td><td>4158</td><td>2520</td><td>1732</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>3289</td><td>1994</td><td>1370</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>2358</td><td>1429</td><td>982</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1782</td><td>1080</td><td>742</td>
    </tr>
    <tr>
        <td rowspan="4">31</td>
        <td rowspan="4">141 x 141</td>
        <td>LOW</td><td>4417</td><td>2677</td><td>1840</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>3486</td><td>2113</td><td>1452</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>2473</td><td>1499</td><td>1030</td>
    </tr>
    <tr>
        <td>HIGH</td><td>1897</td><td>1150</td><td>790</td>
    </tr>
    <tr>
        <td rowspan="4">32</td>
        <td rowspan="4">145 x 145</td>
        <td>LOW</td><td>4686</td><td>2840</td><td>1952</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>3693</td><td>2238</td><td>1538</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>2670</td><td>1618</td><td>1112</td>
    </tr>
    <tr>
        <td>HIGH</td><td>2022</td><td>1226</td><td>842</td>
    </tr>
    <tr>
        <td rowspan="4">33</td>
        <td rowspan="4">149 x 149</td>
        <td>LOW</td><td>4965</td><td>3009</td><td>2068</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>3909</td><td>2369</td><td>1628</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>2805</td><td>1700</td><td>1168</td>
    </tr>
    <tr>
        <td>HIGH</td><td>2157</td><td>1307</td><td>898</td>
    </tr>
    <tr>
        <td rowspan="4">34</td>
        <td rowspan="4">153 x 153</td>
        <td>LOW</td><td>5253</td><td>3183</td><td>2188</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>4134</td><td>2506</td><td>1722</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>2949</td><td>1787</td><td>1228</td>
    </tr>
    <tr>
        <td>HIGH</td><td>2301</td><td>1394</td><td>958</td>
    </tr>
    <tr>
        <td rowspan="4">35</td>
        <td rowspan="4">157 x 157</td>
        <td>LOW</td><td>5529</td><td>3351</td><td>2303</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>4343</td><td>2632</td><td>1809</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>3081</td><td>1867</td><td>1283</td>
    </tr>
    <tr>
        <td>HIGH</td><td>2361</td><td>1431</td><td>983</td>
    </tr>
    <tr>
        <td rowspan="4">36</td>
        <td rowspan="4">161 x 161</td>
        <td>LOW</td><td>5836</td><td>3537</td><td>2431</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>4588</td><td>2780</td><td>1911</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>3244</td><td>1966</td><td>1351</td>
    </tr>
    <tr>
        <td>HIGH</td><td>2524</td><td>1530</td><td>1051</td>
    </tr>
    <tr>
        <td rowspan="4">37</td>
        <td rowspan="4">165 x 165</td>
        <td>LOW</td><td>6153</td><td>3729</td><td>2563</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>4775</td><td>2894</td><td>1989</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>3417</td><td>2071</td><td>1423</td>
    </tr>
    <tr>
        <td>HIGH</td><td>2625</td><td>1591</td><td>1093</td>
    </tr>
    <tr>
        <td rowspan="4">38</td>
        <td rowspan="4">169 x 169</td>
        <td>LOW</td><td>6479</td><td>3927</td><td>2699</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>5039</td><td>3054</td><td>2099</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>3599</td><td>2181</td><td>1499</td>
    </tr>
    <tr>
        <td>HIGH</td><td>2735</td><td>1658</td><td>1139</td>
    </tr>
    <tr>
        <td rowspan="4">39</td>
        <td rowspan="4">173 x 173</td>
        <td>LOW</td><td>6743</td><td>4087</td><td>2809</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>5313</td><td>3220</td><td>2213</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>3791</td><td>2298</td><td>1579</td>
    </tr>
    <tr>
        <td>HIGH</td><td>2927</td><td>1774</td><td>1219</td>
    </tr>
    <tr>
        <td rowspan="4">40</td>
        <td rowspan="4">177 x 177</td>
        <td>LOW</td><td>7089</td><td>4296</td><td>2953</td>
    </tr>
    <tr>
        <td>MEDIUM</td><td>5596</td><td>3391</td><td>2331</td>
    </tr>
    <tr>
        <td>QUARTILE</td><td>3993</td><td>2420</td><td>1663</td>
    </tr>
    <tr>
        <td>HIGH</td><td>3057</td><td>1852</td><td>1273</td>
    </tr>
</table>


Special Thanks
--------------

A HUGE thank you to [Project Nayuki](https://www.nayuki.io/) for the
[QR code C++ library](https://github.com/nayuki/QR-Code-generator/tree/master/cpp)
which was critical in development of this library.


License
-------

MIT License.
