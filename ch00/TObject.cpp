#include "stdafx.h"
#include "TObject.h"


#define PrintFun(X) if(s_output){ std::cout << __FUNCTION__ <<(X)<<"\n";}
// 定义包含相关名字
static const std::vector<std::string> s_buildingWords = {
        "Beam", 
        "Column", 
        "Foundation", 
        "Wall", 
        "Roof",
        "Floor", 
        "Ceiling", 
        "Door", 
        "Window", 
        "Staircase",
        "Balcony", 
        "Terrace", 
        "Pavement", 
        "Concrete", 
        "Steel",
        "Wood", 
        "Brick", 
        "Glass", 
        "Tile", 
        "Marble"
};
// 定义包含相关名字
static const std::vector<int64_t> s_classId = {
        1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20
};
// 初始化包含常见材料
static const std::vector<std::string> s_material = {
    "Steel",
    "Aluminum",
    "Copper",
    "Titanium",
    "Concrete",
    "Glass",
    "Wood",
    "Plastic"
};
// 初始化包含常见电压等级
static const std::vector<Voltage> s_voltage = {
    Voltage::Vol_10kV,
    Voltage::Vol_35kV,
    Voltage::Vol_66kV,
    Voltage::Vol_110kV,
    Voltage::Vol_220kV,
    Voltage::Vol_330kV,
    Voltage::Vol_500kV,
    Voltage::Vol_750kV,
    Voltage::Vol_1000kV,

};
static const std::unordered_map<Voltage,std::string> s_voltageMap = {
    {Voltage::Vol_10kV,"10kV"},
    {Voltage::Vol_35kV,"35kV"},
    {Voltage::Vol_66kV,"66kV"},
    {Voltage::Vol_110kV,"110kV"},
    {Voltage::Vol_220kV,"220kV"},
    {Voltage::Vol_330kV,"330kV"},
    {Voltage::Vol_500kV,"500kV"},
    {Voltage::Vol_750kV,"750kV"},
    {Voltage::Vol_1000kV,"1000kV"},

};
//
// 
static bool s_output = false;
//
static DataKey GetKey(int randomNumber)
{
    static int64_t s_instanceId = 0;
    return  DataKey(s_classId[randomNumber % s_classId.size()], ++s_instanceId);
}
static std::string GetName(int randomNumber)
{
    return  s_buildingWords[randomNumber % s_buildingWords.size()];
}
static std::string GetMaterial(int randomNumber)
{
    return  s_material[randomNumber % s_material.size()];
}
static Voltage GetVoltage(int randomNumber)
{
    return  s_voltage[randomNumber % s_voltage.size()];
}

static void AssignTObject(TObject & tobject)
{
    // 创建一个随机数引擎
    std::random_device rd;
    std::mt19937 gen(rd());
    // 创建一个分布对象
    std::uniform_int_distribution<> dis(0, 1000);
    // 生成一个随机数
    int randomNumber = dis(gen);

    tobject.setDataKey(GetKey(randomNumber));
    tobject.setName(GetName(randomNumber));
    tobject.setMaterial(GetMaterial(randomNumber));  
    tobject.setVoltage(GetVoltage(randomNumber));
    tobject.setNumber(randomNumber);
    tobject.setLength((randomNumber % 1000));
}

TObject::TObject(const DataKey& key)
: m_key(key)
, m_voltage(Voltage::Vol_10kV)
, m_name({})
, m_material({})
, m_number(0)
, m_length(0)
{
    PrintFun("(const DataKey& key)");
}
TObject::TObject() 
: m_key({})
, m_voltage(Voltage::Vol_10kV)
, m_name({})
, m_material({})
, m_number(0)
, m_length(0)
{
    PrintFun("()");
}

TObject::TObject(TObject const& that) 
{
    m_key = that.m_key;
    m_voltage = that.m_voltage;
    m_name = that.m_name;
    m_material = that.m_material;
    m_number =that.m_number;
    m_length = that.m_length;
    PrintFun("(TObject const&)");
}

TObject::TObject(TObject&& that) noexcept
{
   m_key =  that.m_key;
   m_voltage =that.m_voltage;
   m_name = std::move(that.m_name);
   m_material = std::move(that.m_material);
   m_number =  that.m_number;
   m_length = that.m_length;
   PrintFun("(TObject&&)");
}

TObject& TObject::operator=(TObject const& that)  
{
    m_key = that.m_key;
    m_voltage = that.m_voltage;
    m_name = that.m_name;
    m_material = that.m_material;
    m_number = that.m_number;
    m_length = that.m_length;
    PrintFun("(TObject const&)");
    return *this;
}

TObject& TObject::operator=(TObject&& that) noexcept
{
    m_key = that.m_key;
    m_voltage = that.m_voltage;
    m_name = std::move(that.m_name);
    m_material = std::move(that.m_material);
    m_number = that.m_number;
    m_length = that.m_length;
    PrintFun("(TObject&&)");
    return *this;
}

TObject::~TObject()
{
    PrintFun("()");
}

std::vector<TObject> Factory::CreateTObjects(size_t number)
{
    std::vector<TObject> OTbjects;
    OTbjects.reserve(number);
    for (size_t i = 0; i < number; i++)
    {
        OTbjects.push_back(CreateTObject());
    }
    // 创建一个随机数生成器
    std::random_device rd;
    std::mt19937 g(rd());
    // 打乱 vector
    std::shuffle(OTbjects.begin(), OTbjects.end(), g);

    return OTbjects;
}
std::vector<TObjectPtr> Factory::CreateTObjectPtrs(size_t number)
{
    std::vector<TObjectPtr> OTbjectPtrs;
    OTbjectPtrs.reserve(number);
    for (size_t i = 0; i < number; i++)
    {
        OTbjectPtrs.push_back(CreateTObjectPtr());
    }
    // 创建一个随机数生成器
    std::random_device rd;
    std::mt19937 g(rd());
    // 打乱 vector
    std::shuffle(OTbjectPtrs.begin(), OTbjectPtrs.end(), g);
    return OTbjectPtrs;
}
std::vector<TObject> Factory::Ptrs2Objs(const std::vector<TObjectPtr>& tObjPtrs)
{
    std::vector<TObject> tobjs;
    tobjs.reserve(tObjPtrs.size());
    for (size_t i = 0; i < tObjPtrs.size(); i++)
    {
        tobjs.push_back(*tObjPtrs[i]);
    }
    return tobjs;
}
TObject Factory::CreateTObject()
{
    TObject tobject; 
    AssignTObject(tobject);
    return tobject;
}
TObjectPtr Factory::CreateTObjectPtr()
{
    TObjectPtr tPtr = std::make_shared<TObject>();
    AssignTObject(*tPtr);
    return tPtr;
}


void Factory::EnalbeMsg()
{
    s_output = true;
}
void Factory::DisalbeMsg()
{
    s_output = false;

}
std::ostream& operator<<(std::ostream& os, TObject const& tobj)
{
   auto it =  s_voltageMap.find(tobj.m_voltage);
    os << tobj.m_key
        << ", Name: " << std::setw(10) << std::setfill(' ') << tobj.m_name
        << ", Material: "  << std::setw(10) << std::setfill(' ') << tobj.m_material
        << ", Voltage: " << std::setw(6) << std::setfill(' ') << (it->second)
        << ", Number: " << std::setw(6) << std::setfill(' ') << tobj.m_number
        << ", Length: " << std::setw(6) << std::setfill(' ') << tobj.m_length
        << "\n";
    return os;
}
