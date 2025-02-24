#include <iostream>
#include <string>
#include <iomanip> // For std::setw and std::setfill

std::string timeConvertion(const std::string& s){
  
  // Extract hours, minutes, seconds, and period from the input string
  int hh = std::stoi(std::string(s.substr(0, 2))); // Extract hours
  int mm = std::stoi(std::string(s.substr(3, 2))); // Extract minutes
  int ss = std::stoi(std::string(s.substr(6, 2))); // Extract seconds
  std::string period = std::string(s.substr(8, 2)); // Extract "AM" or "PM"
  
  if (period == "PM" && hh != 12) {
        hh += 12;
  } else if (period == "AM" && hh == 12) {
        hh = 0;
  }  
  
  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hh << ":"
      << std::setw(2) << std::setfill('0') << mm << ":"
      << std::setw(2) << std::setfill('0') << ss;

  return oss.str()
  
}

/*
int main() {
  
  std::cout << "Entrada: 07:05:45PM -> Salida: " << timeConversion("07:05:45PM") << std::endl;
  std::cout << "Entrada: 12:00:00AM -> Salida: " << timeConversion("12:00:00AM") << std::endl;

  return 0;
}

*/
