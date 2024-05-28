
namespace internal
{

template<typename T>
class SkewnessFisherPearson
  : public Skewness<T>
{

public:

    SkewnessFisherPearson() : Skewness<T>() {}
    ~SkewnessFisherPearson() override = default;

    auto eval(const DescriptiveStatistics<T> &stat) const -> double override
    {
        size_t n = stat.size();
        
        if(n <= 1) return consts::zero<double>;
        
        double _mean = stat.mean();
        double dif{};
        double skew{};
        
        for(const auto &data : stat.data()) {
            dif = static_cast<double>(data) - _mean;
            skew += dif * dif * dif;
        }
        
        double _variance = stat.variance();
        
        if(_variance == consts::zero<double>) return consts::zero<double>;
        
        if(stat.isSample()) {
            return skew * n / ((n - 1) * (n - 2) * _variance * stat.standarDeviation());
        } else {
            return skew / (n * _variance * stat.standarDeviation());
        }
    }

};


template<typename T>
class SkewnessPearson
  : public Skewness<T>
{

public:

    SkewnessPearson() : Skewness<T>() {}
    ~SkewnessPearson() override = default;

    auto eval(const DescriptiveStatistics<T> &stat) const -> double override
    {
        return (stat.mean() - stat.mode()) / stat.standarDeviation();
    }

};


template<typename T>
class SkewnessBowley
  : public Skewness<T>
{

public:

    SkewnessBowley() : Skewness<T>() {}
    ~SkewnessBowley() override = default;

    auto eval(const DescriptiveStatistics<T> &stat) const -> double override
    {
        return (stat.thirdQuartile() + stat.firstQuartile() - 2 * stat.secondQuartile()) /
        stat.interquartileRange();
    }

};


template<typename T>
class SkewnessMomental
  : public Skewness<T>
{

public:

    SkewnessMomental() : Skewness<T>() {}
    ~SkewnessMomental() override = default;
    
    auto eval(const DescriptiveStatistics<T> &stat) const -> double override
    {
      return 0.0;
    }

};


} // internal


template<typename T>
auto SkewnessFactory<T>::create(SkewnessMethod method) -> std::shared_ptr<Skewness<T>>
{
    std::shared_ptr<Skewness<T>> skew;

    switch (method) {
        case SkewnessMethod::fisher_pearson:
            skew = std::make_shared<internal::SkewnessFisherPearson<T>>();
            break;
        case SkewnessMethod::pearson_mode:
            skew = std::make_shared<internal::SkewnessPearson<T>>();
            break;
        case SkewnessMethod::bowley:
            skew = std::make_shared<internal::SkewnessBowley<T>>();
            break;
        case SkewnessMethod::momental:
            skew = std::make_shared<internal::SkewnessMomental<T>>();
            break;
    }

    return skew;
}