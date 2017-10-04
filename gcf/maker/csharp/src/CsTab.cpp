#include "CsTab.h"
#include "CsUtility.h"
#include "utility/TabVisitor.h"
#include <fstream>
#include <iostream>

GCF_NAMESPACE_BEGIN

namespace cs
{
    static void sWriteData(std::ostream& stream, const std::vector<std::string>& vec, int tab = 1)
    {
        stream << utility::Tab(tab);

        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (i) stream << ",";
            if (i && (i % 5) == 0) stream << std::endl << utility::Tab(tab);
            if (i) stream << " ";

            stream << "\"" << vec[i] << "\"";
        }
    }

    Tab::Tab()
    {
    }

    Tab::~Tab()
    {
    }

    Tab* Tab::Create()
    {
        return new Tab();
    }

    void Tab::Release()
    {
        delete this;
    }

    bool Tab::OnBegin(const IScope* global, const char* path, const char* name)
    {
        _file = utility::ContactPath(path, name) + "_Tab";
        _name = name;
        return true;
    }

    void Tab::OnEnd()
    {
        if (_count == 0)
            return;

        std::ofstream stream(_file + ".cs");
        if (!stream.is_open())
        {
            std::cerr << "can not open file " << _file << ".cs" << std::endl;
            return;
        }

        DeclPrev(stream);
        Decl(stream, &_root, 0);
        DeclTail(stream);
    }

    void Tab::OnType(const IType* type)
    {
        if (type->TypeCat() != TypeCategory::Struct)
            return;

        const IStructType* sTy = static_cast<const IStructType*>(type);
        if (sTy->CfgCat() != CfgCategory::Tab)
            return;

        NsTabs* nsTabs = Record(sTy->Owner());
        assert(nsTabs);

        nsTabs->tabs.push_back(sTy);
        ++_count;
    }

    Tab::NsTabs* Tab::Record(IScope* owner)
    {
        if (owner == nullptr || owner->BindNs() == nullptr || owner->BindType() == nullptr)
            return &_root;

        NsTabs* ns = Record(owner->Owner());
        auto it = std::find_if(ns->children.begin(), ns->children.end(),
            [owner](NsTabs* other) { return other->scope == owner; });

        if (it != ns->children.end())
            return *it;

        NsTabs* rd = new NsTabs();
        rd->scope = owner;
        rd->owner = ns;
        ns->children.push_back(rd);
        return rd;
    }

    void Tab::DeclPrev(std::ostream& stream)
    {
        stream <<
            utility::Tab(0) << "/*" << std::endl <<
            utility::Tab(0) << " * this file is auto generated." << std::endl <<
            utility::Tab(0) << " * please does not edit it manual!" << std::endl <<
            utility::Tab(0) << "*/" << std::endl <<
            utility::Tab(0) << "using System;" << std::endl << std::endl <<
            utility::Tab(0) << "namespace Tab" << std::endl <<
            utility::Tab(0) << "{" << std::endl <<
            utility::Tab(1) << "public class Info" << std::endl <<
            utility::Tab(1) << "{" << std::endl <<
            utility::Tab(2) << "public string[] Titles;" << std::endl <<
            utility::Tab(2) << "public string[] Types;" << std::endl <<
            utility::Tab(2) << "public string[] Descs;" << std::endl <<
            utility::Tab(1) << "}" << std::endl <<
            utility::Tab(0) << "} // namespace Tab" << std::endl;
    }

    void Tab::Decl(std::ostream& stream, Tab::NsTabs* ns, int tab)
    {
        if (ns->scope)
        {
            if (ns->scope->BindNs())
            {
                stream <<
                    utility::Tab(tab) << "namespace " << ns->scope->Name() << std::endl <<
                    utility::Tab(tab) << "{" << std::endl;
                ++tab;
            }
            else if (ns->scope->BindType())
            {
                assert(ns->scope->BindType()->TypeCat() == TypeCategory::Struct);
                stream <<
                    utility::Tab(tab) << "public partial class " << ns->scope->Name() << std::endl <<
                    utility::Tab(tab) << "{" << std::endl;
                ++tab;
            }
        }

        for (auto sTy : ns->tabs)
        {
            Decl(stream, sTy, tab);
            stream << std::endl;
        }

        for (auto child : ns->children)
        {
            stream << std::endl;
            Decl(stream, child, tab);
        }

        if (ns->scope)
        {
            if (ns->scope->BindNs())
            {
                --tab;
                stream <<
                    utility::Tab(tab) << "} // namespace" << ns->scope->Name() << std::endl;
            }
            else if (ns->scope->BindType())
            {
                assert(ns->scope->BindType()->TypeCat() == TypeCategory::Struct);
                --tab;
                stream <<
                    utility::Tab(tab) << "}" << std::endl;
            }
        }
    }

    void Tab::Decl(std::ostream& stream, const IStructType* sTy, int tab)
    {
        utility::TabVisitor visitor;
        utility::Traverse(sTy, &visitor);

        stream <<
            utility::Tab(tab) << "public partial class " << sTy->Name() << std::endl <<
            utility::Tab(tab) << "{" << std::endl;
        ++tab;

        stream << utility::Tab(tab) << "public static Tab.Info TabInfo = new Tab.Info {" << std::endl;
        ++tab;

        stream << utility::Tab(tab) << "Titles = new string[] {" << std::endl;
        sWriteData(stream, visitor.Title(), tab + 1);
        stream << std::endl <<
            utility::Tab(tab) << "}," << std::endl;

        stream << utility::Tab(tab) << "Types = new string[] {" << std::endl;
        sWriteData(stream, visitor.Type(), tab + 1);
        stream << std::endl <<
            utility::Tab(tab) << "}," << std::endl;

        stream << utility::Tab(tab) << "Descs = new string[] {" << std::endl;
        sWriteData(stream, visitor.Describe(), tab + 1);
        stream << std::endl <<
            utility::Tab(tab) << "}" << std::endl;

        --tab;
        stream << utility::Tab(tab) << "};" << std::endl;

        --tab;
        stream << utility::Tab(tab) << "}" << std::endl;
    }

    void Tab::DeclTail(std::ostream& stream)
    {
        // nothing
    }
}

GCF_NAMESPACE_END
