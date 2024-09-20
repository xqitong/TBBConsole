#pragma once

enum class Voltage
{
    Vol_10kV,
    Vol_35kV,
    Vol_66kV,
    Vol_110kV,
    Vol_220kV,
    Vol_330kV,
    Vol_500kV,
    Vol_750kV,
    Vol_1000kV,
};

using ClassId = int64_t;
using InstanceId = int64_t;
class DataKey
{
private:
    ClassId m_ClassId;
    InstanceId m_InstanceId;
public:
    DataKey()
        : m_ClassId(-1LL)
        , m_InstanceId(-1LL)
    {}
    DataKey(ClassId classId, InstanceId const& instanceId)
        : m_ClassId(classId), m_InstanceId(instanceId)
    {}
    bool operator == (DataKey const& other) const
    {
        return m_ClassId == other.m_ClassId && m_InstanceId == other.m_InstanceId;
    }
    bool operator != (DataKey const& other) const
    {
        return m_ClassId != other.m_ClassId || m_InstanceId != other.m_InstanceId;
    }
    bool operator < (DataKey const& other) const
    {
        if (m_ClassId < other.m_ClassId)
        {
            return true;
        }
        if (m_ClassId > other.m_ClassId)
        {
            return false;
        }
        return m_InstanceId < other.m_InstanceId;
        // stl book page96
        //return m_ClassId < other.m_ClassId ||(!(other.m_ClassId < m_ClassId) && m_InstanceId < other.m_InstanceId);
    }
    ClassId getClassId() const { return m_ClassId; }
    InstanceId const& getInstanceId() const { return m_InstanceId; }
    bool isValid() const
    {
        return (m_ClassId > 0LL && m_InstanceId > 0LL);
    }
    // 重载 operator<< 运算符
    friend std::ostream& operator<<(std::ostream& os, DataKey const& dataKey)
    {
        os << std::left << std::setfill(' ') << "ClassId: " << std::setw(3)  << dataKey.m_ClassId  << ", InstanceId: " << std::setw(6) << std::setfill(' ') << dataKey.m_InstanceId ;
        return os;
    }
};
class TObject
{
public:
    // ctor
    TObject();
    TObject(const DataKey & key);  
    // copy ctor
    TObject(TObject const &);
    // move ctor
    TObject(TObject&&) noexcept;
    // copy assign 
    TObject& operator = (TObject const&);
    // move assign 
    TObject& operator = (TObject&&) noexcept;
    // dtor
    ~TObject();

    void setDataKey(DataKey const & value){ m_key = value;}
    DataKey const & getDataKey() const noexcept { return m_key; }
    void setName(std::string const& value) { m_name = value; }
    void setName(std::string && value) { m_name = std::move(value); }
    std::string const&  getName() const noexcept { return m_name; }
    void setMaterial(std::string const& value) { m_material = value; }
    void setMaterial(std::string && value) { m_material = std::move(value); }
    std::string const&  getMaterial() const noexcept { return m_material; }
    void setVoltage(Voltage const& value) { m_voltage = value; }
    Voltage const&  getVoltage() const noexcept { return m_voltage; }
    void setNumber(int value) { m_number = value; }
    int getNumber() const noexcept { return m_number; }
    void setLength(double value) { m_length = value; }
    double getLength() const noexcept { return m_length; }
    // 重载 operator<< 运算符
    friend std::ostream& operator<<(std::ostream& os, TObject const& tobj);

private:
    DataKey m_key;
    Voltage m_voltage;
    std::string m_name;
    std::string m_material;
    int m_number;
    double m_length;
};
using TObjectPtr = std::shared_ptr<TObject>;

class Factory
{
public:
    static std::vector<TObject> CreateTObjects(size_t number);
    static  std::vector<TObjectPtr> CreateTObjectPtrs(size_t number);
    static  std::vector<TObject> Ptrs2Objs(const std::vector<TObjectPtr>& tObjPtrs);
    static  TObject CreateTObject();
    static  TObjectPtr CreateTObjectPtr();
    //
    template<class Fun>
    static double EvalTime(Fun fun, const std::string& name) {
        const auto t1 = std::chrono::high_resolution_clock::now();
        fun();
        const auto t2 = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << std::setw(28) << std::left << name << "："
            << '\t' << "时间：" << ms.count() << " 毫秒\n";
        return  ms.count();
    }
    //
    template<typename T>
    static std::vector<T> CreateArray(size_t size = 10) {
        std::vector<T> a(size);
        for (int i = 0; i < size; ++i)
        {
            a[i] = (T)i;
        }
        return a;
    }
    //
    template<typename T>
    static std::vector<T> CreateEvenArray(size_t size = 10) {
        std::vector<T> a(size);
        for (int i = 0; i < size; ++i)
        {
            a[i] = (T)(2 * i);
        }
        return a;
    }
    //
    template<typename T>
    static std::vector<T> CreateRandomArray(size_t size = 10) {

        std::default_random_engine dre(time(0));
        std::uniform_int_distribution<size_t>uid(0, size);
        std::vector<T> a(size);
        for (int i = 0; i < size; ++i)
        {
            a[i] = (T)(uid(dre));
        }
        return a;
    }
    //
    template <typename T>
    static void MyPrint(const T& container, const std::string& str = "")
    {
        std::cout << str;
        for (auto it = container.cbegin(); it != container.cend(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
    //
    static void EnalbeMsg();
    static void DisalbeMsg();
};


