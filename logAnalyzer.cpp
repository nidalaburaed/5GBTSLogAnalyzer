#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <regex>

struct MeasurementStats {
    double sinr_sum = 0;
    double rsrp_sum = 0;
    double rsrq_sum = 0;
    int count = 0;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <log_file_path>\n";
        return 1;
    }

    std::ifstream logFile(argv[1]);
    if (!logFile.is_open()) {
        std::cerr << "Error opening log file.\n";
        return 1;
    }

    std::string line;
    int rrc_requests = 0, rrc_setups = 0;
    int dl_grants = 0, harq_acks = 0;
    int pdcp_sdu_count = 0, pdcp_sdu_total_size = 0;
    int mcs_sum = 0, mcs_count = 0, rb_sum = 0;

    std::map<int, int> cqi_distribution;
    MeasurementStats stats;

    std::regex regex_rrc_request("RRC Connection Request");
    std::regex regex_rrc_setup("RRC Connection Setup completed");
    std::regex regex_dl_grant("DL grant sent.*MCS=(\\d+), RBs=(\\d+)");
    std::regex regex_harq_ack("HARQ ACK received");
    std::regex regex_pdcp_sdu("SDU size=(\\d+)");
    std::regex regex_cqi("CQI=(\\d+)");
    std::regex regex_meas("RSRP=([-\\d.]+) dBm, RSRQ=([-\\d.]+) dB, SINR=([-\\d.]+) dB");

    while (std::getline(logFile, line)) {
        if (std::regex_search(line, regex_rrc_request)) rrc_requests++;
        if (std::regex_search(line, regex_rrc_setup)) rrc_setups++;
        if (std::regex_search(line, regex_harq_ack)) harq_acks++;

        std::smatch match;
        if (std::regex_search(line, match, regex_dl_grant)) {
            dl_grants++;
            mcs_sum += std::stoi(match[1]);
            mcs_count++;
            rb_sum += std::stoi(match[2]);
        }

        if (std::regex_search(line, match, regex_pdcp_sdu)) {
            pdcp_sdu_total_size += std::stoi(match[1]);
            pdcp_sdu_count++;
        }

        if (std::regex_search(line, match, regex_cqi)) {
            int cqi = std::stoi(match[1]);
            cqi_distribution[cqi]++;
        }

        if (std::regex_search(line, match, regex_meas)) {
            stats.rsrp_sum += std::stod(match[1]);
            stats.rsrq_sum += std::stod(match[2]);
            stats.sinr_sum += std::stod(match[3]);
            stats.count++;
        }
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n--- 5G Base Station KPI Summary ---\n\n";
    std::cout << "RRC Setup Success Rate: ";
    std::cout << (rrc_requests > 0 ? 100.0 * rrc_setups / rrc_requests : 0.0) << "%\n";

    std::cout << "HARQ ACK Success Rate (DL): ";
    std::cout << (dl_grants > 0 ? 100.0 * harq_acks / dl_grants : 0.0) << "%\n";

    std::cout << "Average PDCP Uplink SDU Size: ";
    std::cout << (pdcp_sdu_count > 0 ? pdcp_sdu_total_size / pdcp_sdu_count : 0) << " bytes\n";

    std::cout << "Average SINR: " << (stats.count > 0 ? stats.sinr_sum / stats.count : 0.0) << " dB\n";
    std::cout << "Average RSRP: " << (stats.count > 0 ? stats.rsrp_sum / stats.count : 0.0) << " dBm\n";
    std::cout << "Average RSRQ: " << (stats.count > 0 ? stats.rsrq_sum / stats.count : 0.0) << " dB\n";

    std::cout << "Average MCS: " << (mcs_count > 0 ? static_cast<double>(mcs_sum) / mcs_count : 0.0) << "\n";
    std::cout << "Average Resource Blocks (RBs): " << (dl_grants > 0 ? static_cast<double>(rb_sum) / dl_grants : 0.0) << "\n";

    std::cout << "\nCQI Distribution:\n";
    for (const auto& [cqi, count] : cqi_distribution) {
        std::cout << "  CQI " << cqi << ": " << count << "\n";
    }

    return 0;
}