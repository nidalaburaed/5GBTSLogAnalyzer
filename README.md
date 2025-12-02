# Description

![image](https://github.com/user-attachments/assets/4bfda9c3-f075-4622-96a1-6bdb811be203)

# Functional view

##### C++-based script that analyzes 5G NR (vRAN) call-related log data file (.txt) content and derives common Network KPIs from them.
##### The script is designed to support network operator analyzing 5G base station call quality
##### The script calculates the following KPIs:

 	•	RRC Setup Success Rate, HARQ ACK Success Rate (DL), Average PDCP Uplink SDU Size, Average SINR, Average RSRP, Average RSRQ, Average MCS, Average Resource Blocks and CQI Distribution

##### The script is helping to identify these common problems in 5G Systems:

##### 1. Radio Network Level (Typical Causes)

##### a. Coverage Holes / Interference
	•	Caused by physical obstructions, weather, or misaligned antennas.
	•	Often localized and can be seen with spike patterns in certain cells.

#### b. Handover Failures
	•	If X2/N2 handover fails due to bad target cell config or signaling issues.
	•	Sudden drop in mobility success + spike in drops.

#### c. Timing Issues
	•	TA (Timing Advance) misconfigurations leading to RLF (Radio Link Failure).
	•	May relate to incorrect synchronization with GPS/1588 clocks.

#### d. Excessive Load / Resource Congestion
	•	PRB (Physical Resource Block) exhaustion.
	•	High UE density with insufficient scheduling capacity or license limits.

#### e. HARQ/NACK Issues
	•	HARQ processes failing due to bad link adaptation or interference.
	•	Low DL BLER + drop spike can be a hint.

⸻

#### 2. Core Network / Interface Issues

Less common, but worth checking during area-wide spikes.

#### a. Session Management Fails (SMF/UPF)
	•	If PDU session fails mid-call, the drop appears to radio as an RLF.
	•	Look at NG-C signaling failures (N2 interface).

#### b. AMF Paging or Registration Rejection
	•	If mobility is blocked due to identity mismatch or TAC issues.

⸻

#### 3. Server/Software-Level Causes (on the gNB side)

#### a. Memory Cache Exhaustion or Leaks
	•	TCP/IP stack buffer issues, EPC anchor process leak, or bad LRU policies.
	•	Often seen with slowly building issues until sudden tipping point.

#### b. Thread Starvation or Core Overload
	•	gNB processing threads over-allocated or stuck (like RRC or MAC threads).
	•	High context switch rate, high core temperature, or stuck mutexes.

#### c. gNB Application or FAPI Driver Fault
	•	Abnormal behavior from PHY/MAC interface (via FAPI/IFx).
	•	Related to real-time scheduling or DPDK-related drops.

⸻

#### 4. Transport / Backhaul Issues
	•	High latency, jitter, or packet loss between DU and CU.
	•	Common in split option 7.x or 2 when fronthaul is not guaranteed low-latency.

⸻

#### 5. Software Upgrade/Config Anomalies
	•	Bad config pushed via EMS (wrong PCI/TA/NRARFCN).
	•	Incomplete restart after upgrade, e.g., partial component active.

# DEMO

```bash
$ ./logAnalyzer normal_data.txt

--- 5G Base Station KPI Summary ---

RRC Setup Success Rate: 100.00%
HARQ ACK Success Rate (DL): 100.00%
Average PDCP Uplink SDU Size: 186.00 bytes
Average SINR: 19.54 dB
Average RSRP: -89.57 dBm
Average RSRQ: -10.05 dB
Average MCS: 18.64
Average Resource Blocks (RBs): 48.85

CQI Distribution:
  CQI 5: 7
  CQI 6: 11
  CQI 7: 4
  CQI 8: 7
  CQI 9: 11
  CQI 10: 3
  CQI 11: 11
  CQI 12: 5
  CQI 13: 17
  CQI 14: 9
  CQI 15: 15

```

# Instructions to compile and run the code

Compile the code using the following command:

```bash
g++ -o logAnalyzer logAnalyzer.cpp .
```

Run the code using the following command:

```bash
./logAnalyzer <log-file.txt>
```

NOTE. You need atleast C++17 (or newer) compiler to support the compilation. If you can't compile the tool on your local workstation, another option is to compile the tool on some online-based IDE

# Next steps

Log file conversion from .txt to csv while doing analysis - file conversion feature

# Notes

Example log file (.txt) is provided in the repository for testing purposes

## Inquiries

In case of inquiries, feel free to send a message via email to nidalaburaed94@gmail.com



