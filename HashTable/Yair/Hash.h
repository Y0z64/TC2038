template <class T>
class Hash
{
private:
    bool ultraSecure = true;
public:
    Hash();
    Hash(const bool ultraSecure);

    int hash(const T& key, const int size);
};