#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>

class Measurement {
    float _value;
    float _error;
public:
    // Construtor
    Measurement(float value = 0, float error = 0)
        : _value(value), _error(error) {};
    // Leitura
    std::tuple<float, float> value_error() const {
        return {_value, _error};
    }

    friend Measurement operator+(Measurement const &, Measurement const &);
    friend Measurement operator-(Measurement const &, Measurement const &);
    friend Measurement operator*(Measurement const &, Measurement const &);
    friend Measurement operator/(Measurement const &, Measurement const &);

    friend std::ostream &operator<<(std::ostream &, Measurement const &);
    friend std::istream &operator>>(std::istream &, Measurement &);
};


struct ParticlePosition {
  Measurement time;   // Time
  Measurement height; // Associated height
};

//-----------------------------------------------------------------------------
//
// Auxiliary types and functions for the main function.
//

// Some useful type synonyms.
using Positions = std::vector<ParticlePosition>;
using Velocities = std::vector<Measurement>;

// Tells how to execute the code.
void usage(std::string exename);

// Reads data from filename.
Positions read_data(std::string filename);

// Computes the value of g given the time and height data.
Measurement compute_g(Positions data);

// Compute velocities in each instant given the data and
// already evaluated g.
Velocities compute_velocities(Positions data, Measurement g);


// main

int main(int argc, char const *argv[]) {
  // We need an argument with the name of the data file.
  if (argc != 2) {
    usage(argv[0]);
    std::exit(1);
  }

  auto data = read_data(argv[1]);

  auto g = compute_g(data);

  auto velocities = compute_velocities(data, g);

  std::cout << "Evaluated values follow.\n\n";
  std::cout << "Gravitational acceleration: " << g << std::endl;
  std::cout << "Velocities:\n";
  for (size_t i = 0; i < velocities.size(); ++i) {
    std::cout << velocities[i] << std::endl;
  }

  return 0;
}


void usage(std::string exename) {
  std::cerr << "Usage: " << exename << " <data file name>\n";
}

// Operadores Aritméticos
inline float square(float x) { return x * x; }

Measurement operator+(Measurement const &a, Measurement const &b){
    auto value = a._value + b._value;
    auto error = std::sqrt(square(a._error) + square(b._error));
    return {value, error};
}

Measurement operator-(Measurement const &a, Measurement const &b){
    auto value = a._value - b._value;
    auto error = std::sqrt(square(a._error) + square(b._error));
    return {value, error};
}

Measurement operator*(Measurement const &a, Measurement const &b){
    auto value = a._value * b._value;
    auto error = std::fabs(value) * std::sqrt(square(a._error / a._value) + square(b._error / b._value));
    return {value, error};
}

Measurement operator/(Measurement const &a, Measurement const &b){
    auto value = a._value / b._value;
    auto error = std::fabs(value) * std::sqrt(square(a._error / a._value) + square(b._error / b._value));
    return {value, error};
}

// Operador de inserção
std::ostream &operator<<(std::ostream &os, Measurement const &a) {
  // Escrevemos no formato value +- error
  os << a._value << " +- " << a._error;
  return os;
}

// Operador de extração
std::istream &operator>>(std::istream &is, Measurement &m) {
    float v, e;
    char sep;

    is >> v;
    if (!is.good())
        return is;

    is >> sep;
    if (!is.good())
        return is;
    if (sep != '+') {
        is.setstate(std::ios::failbit);
        return is;
    }
    is >> sep;
    if (!is.good())
        return is;
    if (sep != '-') {
        is.setstate(std::ios::failbit);
        return is;
    }

    is >> e;
    if (!is.good())
        return is;

    m = Measurement{v, e};
    return is;
}

Positions read_data(std::string filename) {

    Positions data;

    std::ifstream datafile(filename);
    if (!datafile.good()) {
        std::cerr << "Error reading " << filename << std::endl;
        std::exit(2);
    }

    // Read a position (time+height with errors) value.
    float value, error;
    // Try to read until the end of the file.
    while (datafile >> value) {
        // If we find a value, there must be 3 more values.

        datafile >> error;
        if (datafile.fail()) {
            std::cerr << "Error reading data from " << filename << std::endl;
            std::exit(3);
        }

        Measurement time{value, error};

        datafile >> value;
        datafile >> error;
        if (datafile.fail()) {
            std::cerr << "Error reading data from " << filename << std::endl;
            std::exit(3);
        }

        Measurement height{value, error};

        data.push_back({time, height});
    }

    return data;
}

// Computes the value of g given the time and height data.
Measurement compute_g(Positions data) {
  // Uses the first, second and last positions and corresponding times,
  // and compute
  //
  // 2 [(hn-h1)t0-(hn-h0)t1+(h1-h0)tn] / [(t1-t0)(tn-t1)(tn-t0)]
  //
  // (where t is time, h is height and 0, 1, n indicate first, second and last
  // points.)
  auto num_points = data.size();
  auto t0 = data[0].time;
  auto t1 = data[1].time;
  auto tn = data[num_points - 1].time;
  auto h0 = data[0].height;
  auto h1 = data[1].height;
  auto hn = data[num_points - 1].height;

  auto delta_h_10 = h1-h0;
  auto delta_h_n0 = hn-h0;
  auto delta_h_n1 = hn-h1;
  auto delta_t_10 = t1-t0;
  auto delta_t_n0 = tn-t0;
  auto delta_t_n1 = tn-t1;
  auto factor1 = delta_h_n1*t0;
  auto factor2 = delta_h_n0*t1;
  auto factor3 = delta_h_10*tn;
  auto numerator = factor1-factor2+factor3;
  auto denominator = delta_t_10*delta_t_n1*delta_t_n0;
  return 2.0f*(numerator/denominator);
}

// Compute velocities in each instant given the data and
// already evaluated g.
Velocities compute_velocities(Positions data, Measurement g) {
  auto const n_data = data.size();
  Velocities velocities(n_data);

  // For each data point (except the last, see below), evaluate the velocity as
  // the starting velocity for a free fall to reach the next point.
  //
  // v = delta_h/delta_t + g*delta_t/2
  //
  for (size_t i = 0; i < n_data - 1; ++i) {
    auto delta_h = data[i + 1].height-data[i].height;
    auto delta_t = data[i + 1].time-data[i].time;
    velocities[i] = (delta_h/delta_t)+((g*delta_t)/2.0f);
  }

  // The last velocity is evaluated from the one before last and the value of g.
  auto last_delta_t = data[n_data - 1].time-data[n_data - 2].time;
  velocities[n_data - 1] = velocities[n_data - 2]-(g*last_delta_t);

  return velocities;
}
