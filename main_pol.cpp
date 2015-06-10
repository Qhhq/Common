
void dump(int s)
{
    std::cout << s << std::endl;
}

struct Dumpable
{
    virtual void dump() const = 0;
};

template<typename T>
class ConcreteDumpable: public Dumpable
{
public:
    ConcreteDumpable (const T& value)
        : m_value(value)
    {}

    virtual void dump() const override
    {
        ::dump(m_value);
    }
private:
    const T &m_value;
};

class Dumper
{
    using DumpablePtr = std::unique_ptr<Dumpable>;
public: template<typename T>
    void add(const T& obj)
    {
        dumpables.push_back( DumpablePtr(new ConcreteDumpable<T>(obj)));
    }
    void dumpAll() const
    {
        for (const auto &d : dumpables)
        {
            d->dump();
        }
    }
private:
    std::vector<DumpablePtr> dumpables;

};


int main(int argc, char *argv[])
{
    Dumper dumper;
    dumper.add(10);
    dumper.dumpAll();
    return 0;
}
