/// \cond

namespace internal
{

template<typename T>
class KurtosisB2
  : public Kurtosis<T>
{

public:

    KurtosisB2() : Kurtosis<T>() {}
    ~KurtosisB2() override = default;

    auto eval(const DescriptiveStatistics<T> &stat) const -> double override
    {
        size_t n = stat.size();

        if (n <= 1) return consts::zero<double>;

        double _mean = stat.mean();
        double _kurtosis{};

        for (const auto &data : stat.data()) {
            double dif = static_cast<double>(data) - _mean;
            _kurtosis += std::pow(dif, 4);
        }

        double _variance = stat.variance();

        if (isNearlyZero(_variance)) return consts::zero<double>;

        if (stat.isSample()) {
            _kurtosis = static_cast<T>(n) * static_cast<double>(n + 1) * _kurtosis /
                (static_cast<T>(n - 1) * static_cast<T>(n - 2) * static_cast<T>(n - 3) * _variance * _variance);
        } else {
            _kurtosis = _kurtosis /
                (static_cast<T>(n) * _variance * _variance);
        }

        return _kurtosis;
    }

};

template<typename T>
class KurtosisMoors
    : public Kurtosis<T>
{

public:

    KurtosisMoors() : Kurtosis<T>() {}
    ~KurtosisMoors() override = default;

    auto eval(const DescriptiveStatistics<T> &stat) const -> double override
    {
        if (stat.size() < 8) return consts::zero<double>; // Se necesitan al menos 8 valores para calcular octiles

        auto octiles = stat.octiles();

        double q1 = octiles.at(0);
        double q2 = octiles.at(1);
        double q3 = octiles.at(2);
        double q5 = octiles.at(3);
        double q6 = octiles.at(5);
        double q7 = octiles.at(6);

        double numerator = (q7 - q5) + (q3 - q1);
        double denominator = q6 - q2;

        if (isNearlyZero(denominator)) return consts::zero<double>;

        return numerator / denominator;
    }
};

} // internal

/// \endcond

template<typename T> inline
auto KurtosisFactory<T>::create(KurtosisMethod method) -> std::shared_ptr<Kurtosis<T>>
{
    std::shared_ptr<Kurtosis<T>> kurtosis;

    switch (method) {
        case KurtosisMethod::pearson:
            kurtosis = std::make_shared<internal::KurtosisB2<T>>();
            break;
        case KurtosisMethod::moors:
            kurtosis = std::make_shared<internal::KurtosisMoors<T>>();
            break;
        default:
            kurtosis = nullptr;
            break;
    }

    return kurtosis;
}

