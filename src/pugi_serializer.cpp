/**
 * xml serializer based on pugi parser - version 0.1
 * --------------------------------------------------------
 * Copyright (C) 2021, by Shai Shsag (shaishasag@yahoo.co.uk)
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugi xml parser, which is:
 * Copyright (C) 2006, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 */

#ifndef SOURCE_PUGI_SERIALIZER_CPP
#define SOURCE_PUGI_SERIALIZER_CPP

#include "pugi_serializer.hpp"

namespace pugi_serializer
{
    
namespace impl
{

class impl_base
{
public:
    impl_base() = default;
    virtual ~impl_base() = default;
    
    impl_base(const impl_base&) = default;
    impl_base& operator=(const impl_base&) = default;
    impl_base(impl_base&&) = default;
    impl_base& operator=(impl_base&&) = default;

    bool reading() const { return _reading;}
    bool writing() const { return !_reading;}
    
    virtual void node_name(pugi::xml_node _node, std::string& _name) = 0;
    virtual pugi::xml_node child(pugi::xml_node _node, const char* _name) = 0;
    virtual pugi::xml_node next_sibling(pugi::xml_node _node, const char* _name) = 0;

    virtual void text(pugi::xml_node _node, std::string& _text) = 0;
    virtual void text(pugi::xml_node _node, std::string& _text, const char* default_text) = 0;
    virtual void text(pugi::xml_node _node, int& _int) = 0;
    virtual void text(pugi::xml_node _node, int& _int, const int def) = 0;
    virtual void text(pugi::xml_node _node, unsigned& _uint) = 0;
    virtual void text(pugi::xml_node _node, unsigned& _uint, const unsigned def) = 0;
    virtual void text(pugi::xml_node _node, float& _float) = 0;
    virtual void text(pugi::xml_node _node, float& _float, const float def) = 0;
    virtual void text(pugi::xml_node _node, double& _double) = 0;
    virtual void text(pugi::xml_node _node, double& _double, const double def) = 0;
    virtual void text(pugi::xml_node _node, bool& _bool) = 0;
    virtual void text(pugi::xml_node _node, bool& _bool, const bool def) = 0;
    virtual void text(pugi::xml_node _node, long long& _llint) = 0;
    virtual void text(pugi::xml_node _node, long long& _llint, const long long def) = 0;
    virtual void text(pugi::xml_node _node, unsigned long long& _ullint) = 0;
    virtual void text(pugi::xml_node _node, unsigned long long& _ullint, const unsigned long long def) = 0;

    virtual void attribute(pugi::xml_node _node, const char* _name, std::string& _text) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, std::string& _text, const char* default_text) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, int& _int) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, int& _int, const int def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, unsigned& _uint) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, unsigned& _uint, const unsigned def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, float& _float) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, float& _float, const float def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, double& _double) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, double& _double, const double def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, bool& _bool) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, bool& _bool, const bool def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, long long& _llint) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, long long& _llint, const long long def) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, unsigned long long& _ullint) = 0;
    virtual void attribute(pugi::xml_node _node, const char* _name, unsigned long long& _ullint, const unsigned long long def) = 0;

protected:
    bool _reading = true;
};

class writer_impl : public impl_base
{
public:
    
    writer_impl()
    {
        _reading = false;
    }
    
    void node_name(pugi::xml_node _node, std::string& _name) override
    {
        _node.set_name(_name.c_str());
    }

    pugi::xml_node child(pugi::xml_node _node, const char* _name) override
    {
        auto new_node = _node.append_child(_name);
        return new_node;
    }

    pugi::xml_node next_sibling(pugi::xml_node older_sibling, const char* _name) override
    {
        auto younger_sibling = older_sibling.parent().insert_child_after(_name, older_sibling);
        return younger_sibling;
    }

    void text(pugi::xml_node _node, std::string& _text) override
    {
        _node.text().set(_text.c_str());
    }
    
    void text(pugi::xml_node _node, std::string& _text, const char* default_text) override
    {
        if (_text != default_text)
        {
            text(_node, _text);
        }
    }
    
    template<typename TToWrite>
    void write_node_value(pugi::xml_node _node, TToWrite& _val)
    {
        _node.text().set(_val);
    }
    
    template<typename TToWrite>
    void write_node_value_with_default(pugi::xml_node _node, TToWrite& _val, const TToWrite def)
    {
        if (def != _val)
            write_node_value(_node, _val);
    }

    void text(pugi::xml_node _node, int& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, int& _val, const int def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, unsigned& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, unsigned& _val, const unsigned def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, float& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, float& _val, const float def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, double& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, double& _val, const double def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, bool& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, bool& _val, const bool def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, long long& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, long long& _val, const long long def) override
        { write_node_value_with_default(_node, _val, def); }
    
    void text(pugi::xml_node _node, unsigned long long& _val) override
        { write_node_value(_node, _val); }
    void text(pugi::xml_node _node, unsigned long long& _val, const unsigned long long def) override
        { write_node_value_with_default(_node, _val, def); }
    

    void attribute(pugi::xml_node _node, const char* _name, std::string& _text) override
    {
        _node.append_attribute(_name) = _text.c_str();
    }
    
    // do not append the attribute if _text is equal to default_text
    void attribute(pugi::xml_node _node, const char* _name, std::string& _text, const char* default_text) override
    {
        if (_text != default_text)
            _node.append_attribute(_name) = _text.c_str();
    }
    
    template<typename TToWrite>
    void write_attribute_value(pugi::xml_node _node, const char* _name, TToWrite& _to_write)
    {
        _node.append_attribute(_name).set_value(_to_write);
    }
    
    template<typename TToWrite>
    void write_attribute_value_with_default(pugi::xml_node _node, const char* _name, TToWrite& _to_write, const TToWrite def)
    // do not write the value if it's equal to the default
    {
        if (_to_write != def)
        {
            write_attribute_value(_node, _name, _to_write);
        }
    }

    void attribute(pugi::xml_node _node, const char* _name, int& _val) override
        { write_attribute_value(_node, _name, _val); }
    void attribute(pugi::xml_node _node, const char* _name, int& _val, const int def) override
    { write_attribute_value_with_default(_node, _name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _name, unsigned& _val) override
        { write_attribute_value(_node, _name, _val); }
    void attribute(pugi::xml_node _node, const char* _name, unsigned& _val, const unsigned def) override
        { write_attribute_value_with_default(_node, _name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _name, float& _val) override
        { write_attribute_value(_node, _name, _val); }
    void attribute(pugi::xml_node _node, const char* _name, float& _val, const float def) override
        { write_attribute_value_with_default(_node, _name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _name, double& _val) override
        { write_attribute_value(_node, _name, _val); }
    void attribute(pugi::xml_node _node, const char* _name, double& _val, const double def) override
        { write_attribute_value_with_default(_node, _name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _name, bool& _val) override
        { write_attribute_value(_node, _name, _val); }
    void attribute(pugi::xml_node _node, const char* _name, bool& _val, const bool def) override
        { write_attribute_value_with_default(_node, _name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _name, long long& _val) override
        { write_attribute_value(_node, _name, _val); }
    void attribute(pugi::xml_node _node, const char* _name, long long& _val, const long long def) override
        { write_attribute_value_with_default(_node, _name, _val, def); }
    
    void attribute(pugi::xml_node _node, const char* _name, unsigned long long& _val) override
        { write_attribute_value(_node, _name, _val); }
    void attribute(pugi::xml_node _node, const char* _name, unsigned long long& _val, const unsigned long long def) override
        { write_attribute_value_with_default(_node, _name, _val, def); }
};

class reader_impl : public impl_base
{
public:
    
    void node_name(pugi::xml_node _node, std::string& _name) override
    {
        _name = _node.name();
    }

    pugi::xml_node child(pugi::xml_node _node, const char* _name) override
    {
        auto a_child_node = _node.child(_name);
        return a_child_node;
    }

    pugi::xml_node next_sibling(pugi::xml_node older_sibling, const char* _name) override
    {
        auto younger_sibling = older_sibling.next_sibling(_name);
        return younger_sibling;
    }

    void text(pugi::xml_node _node, std::string& _text) override
    {
        _text = _node.text().as_string();
    }
    
    void text(pugi::xml_node _node, std::string& _text, const char* default_text) override
    {
        if (_node.text())
        {
            _text = _node.value();
        }
        else
        {
            _text = default_text;
        }
    }
    
    void text(pugi::xml_node _node, int& _val) override
    { _val = _node.text().as_int(); }
    void text(pugi::xml_node _node, int& _val, const int def) override
    { if (def != _val) text(_node, _val); }
    
    void text(pugi::xml_node _node, unsigned& _val) override
    { _val = _node.text().as_int(); }
    void text(pugi::xml_node _node, unsigned& _val, const unsigned def) override
    { if (def != _val) text(_node, _val); }
    
    void text(pugi::xml_node _node, float& _val) override
    { _val = _node.text().as_int(); }
    void text(pugi::xml_node _node, float& _val, const float def) override
    { if (def != _val) text(_node, _val); }
    
    void text(pugi::xml_node _node, double& _val) override
    { _val = _node.text().as_int(); }
    void text(pugi::xml_node _node, double& _val, const double def) override
    { if (def != _val) text(_node, _val); }
    
    void text(pugi::xml_node _node, bool& _val) override
    { _val = _node.text().as_int(); }
    void text(pugi::xml_node _node, bool& _val, const bool def) override
    { if (def != _val) text(_node, _val); }
    
    void text(pugi::xml_node _node, long long& _val) override
    { _val = _node.text().as_int(); }
    void text(pugi::xml_node _node, long long& _val, const long long def) override
    { if (def != _val) text(_node, _val); }
    
    void text(pugi::xml_node _node, unsigned long long& _val) override
    { _val = _node.text().as_int(); }
    void text(pugi::xml_node _node, unsigned long long& _val, const unsigned long long def) override
    { if (def != _val) text(_node, _val); }

    void attribute(pugi::xml_node _node, const char* _name, std::string& _text) override
    {
        _text = _node.attribute(_name).as_string(_text.c_str());
    }
    
    // return default_text if attribute does not exists
    void attribute(pugi::xml_node _node, const char* _name, std::string& _text, const char* default_text) override
    {
        if (auto attrib = _node.attribute(_name); attrib)
            _text = attrib.as_string(_text.c_str());
        else
            _text = default_text;
    }
    
    void attribute(pugi::xml_node _node, const char* _name, int& _int) override
    {
        _int = _node.attribute(_name).as_int(_int);
    }
    void attribute(pugi::xml_node _node, const char* _name, int& _int, const int def) override
    {
        _int = _node.attribute(_name).as_int(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _name, unsigned& _uint) override
    {
        _uint = _node.attribute(_name).as_uint(_uint);
    }
    void attribute(pugi::xml_node _node, const char* _name, unsigned& _uint, const unsigned def) override
    {
        _uint = _node.attribute(_name).as_uint(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _name, float& _float) override
    {
        _float = _node.attribute(_name).as_float(_float);
    }
    void attribute(pugi::xml_node _node, const char* _name, float& _float, const float def) override
    {
        _float = _node.attribute(_name).as_float(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _name, double& _double) override
    {
        _double = _node.attribute(_name).as_double(_double);
    }
    void attribute(pugi::xml_node _node, const char* _name, double& _double, const double def) override
    {
        _double = _node.attribute(_name).as_double(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _name, bool& _bool) override
    {
        _bool = _node.attribute(_name).as_bool(_bool);
    }
    void attribute(pugi::xml_node _node, const char* _name, bool& _bool, const bool def) override
    {
        _bool = _node.attribute(_name).as_bool(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _name, long long& _llint) override
    {
        _llint = _node.attribute(_name).as_llong(_llint);
    }
    void attribute(pugi::xml_node _node, const char* _name, long long& _llint, const long long def) override
    {
        _llint = _node.attribute(_name).as_llong(def);
    }
    
    void attribute(pugi::xml_node _node, const char* _name, unsigned long long& _ullint) override
    {
        _ullint = _node.attribute(_name).as_ullong(_ullint);
    }
    void attribute(pugi::xml_node _node, const char* _name, unsigned long long& _ullint, const unsigned long long def) override
    {
        _ullint = _node.attribute(_name).as_ullong(def);
    }
};
} // namespace impl

serilaizer_base::serilaizer_base(pugi::xml_node in_node, impl::impl_base& in_implementor)
: _curr_node(in_node)
, _implementor(in_implementor)
{}
    
serilaizer_base& serilaizer_base::operator=(const serilaizer_base& other)
{
    if (this != & other)
    {
        _curr_node = other._curr_node;
        _implementor = other._implementor;
    }
    
    return *this;
}


bool serilaizer_base::reading() const { return _implementor.reading();}
bool serilaizer_base::writing() const { return _implementor.writing();}
 
void serilaizer_base::node_name(std::string& _name)
{
    _implementor.node_name(_curr_node, _name);
}

serilaizer_base serilaizer_base::child(const char* _name)
{
    pugi::xml_node a_node = _implementor.child(_curr_node, _name);
    return serilaizer_base(a_node, _implementor);
}

serilaizer_base serilaizer_base::next_sibling(const char* _name)
{
    pugi::xml_node a_node = _implementor.next_sibling(_curr_node, _name);
    return serilaizer_base(a_node, _implementor);
}

void serilaizer_base::text(std::string& _text)
{
    _implementor.text(_curr_node, _text);
}

void serilaizer_base::text(std::string& _text, const char* default_text)
{
    _implementor.text(_curr_node, _text, default_text);
}
    
void serilaizer_base::text(int& _val)
{
    _implementor.text(_curr_node, _val);
}

void serilaizer_base::text(int& _val, const int def)
{
}

void serilaizer_base::text(unsigned& _val)
{
    _implementor.text(_curr_node, _val);
}

void serilaizer_base::text(unsigned& _val, const unsigned def)
{
}

void serilaizer_base::text(float& _val)
{
}

void serilaizer_base::text(float& _val, const float def)
{
}

void serilaizer_base::text(double& _val)
{
}

void serilaizer_base::text(double& _val, const double def)
{
}

void serilaizer_base::text(bool& _val)
{
}

void serilaizer_base::text(bool& _val, const bool def)
{
}

void serilaizer_base::text(long long& _val)
{
}

void serilaizer_base::text(long long& _val, const long long def)
{
}

void serilaizer_base::text(unsigned long long& _val)
{
}

void serilaizer_base::text(unsigned long long& _val, const unsigned long long def)
{
    
}

void serilaizer_base::attribute(const char* _name, std::string& _text)
{
    _implementor.attribute(_curr_node, _name, _text);
}

void serilaizer_base::attribute(const char* _name, std::string& _text, const char* def)
{
    _implementor.attribute(_curr_node, _name, _text, def);
}


writer::writer(pugi::xml_document& doc, const char* doc_element_name)
: serilaizer_base(doc.append_child(doc_element_name), *new impl::writer_impl)
{
    
}

writer::writer(pugi::xml_node in_node)
: serilaizer_base(in_node, *new impl::writer_impl)
{}

writer::~writer()
{
    delete & _implementor;
}

reader::reader(pugi::xml_document& doc)
: serilaizer_base(doc.document_element(), *new impl::reader_impl) {}

reader::reader(pugi::xml_node in_node)
: serilaizer_base(in_node, *new impl::reader_impl) {}

reader::~reader()
{
    delete & _implementor;
}

}  // namespace pugi_serializer

#endif // SOURCE_PUGI_SERIALIZER_CPP

/**
 * Copyright (C) 2021, by Shai Shsag (shaishasag@yahoo.co.uk)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */