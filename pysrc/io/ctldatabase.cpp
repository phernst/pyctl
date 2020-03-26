#include <pybind11/pybind11.h>
#include <io/ctldatabase.h>

namespace py = pybind11;

void init_ctldatabase(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    {
        using namespace database;
        auto db { m.def_submodule("database") };
        py::enum_<Element>(db, "Element")
            .value("H", Element::H)
            .value("He", Element::He)
            .value("Li", Element::Li)
            .value("Be", Element::Be)
            .value("B", Element::B)
            .value("C", Element::C)
            .value("N", Element::N)
            .value("O", Element::O)
            .value("F", Element::F)
            .value("Ne", Element::Ne)
            .value("Na", Element::Na)
            .value("Mg", Element::Mg)
            .value("Al", Element::Al)
            .value("Si", Element::Si)
            .value("P", Element::P)
            .value("S", Element::S)
            .value("Cl", Element::Cl)
            .value("Ar", Element::Ar)
            .value("K", Element::K)
            .value("Ca", Element::Ca)
            .value("Sc", Element::Sc)
            .value("Ti", Element::Ti)
            .value("V", Element::V)
            .value("Cr", Element::Cr)
            .value("Mn", Element::Mn)
            .value("Fe", Element::Fe)
            .value("Co", Element::Co)
            .value("Ni", Element::Ni)
            .value("Cu", Element::Cu)
            .value("Zn", Element::Zn)
            .value("Ga", Element::Ga)
            .value("Ge", Element::Ge)
            .value("As", Element::As)
            .value("Se", Element::Se)
            .value("Br", Element::Br)
            .value("Kr", Element::Kr)
            .value("Rb", Element::Rb)
            .value("Sr", Element::Sr)
            .value("Y", Element::Y)
            .value("Zr", Element::Zr)
            .value("Nb", Element::Nb)
            .value("Mo", Element::Mo)
            .value("Tc", Element::Tc)
            .value("Ru", Element::Ru)
            .value("Rh", Element::Rh)
            .value("Pd", Element::Pd)
            .value("Ag", Element::Ag)
            .value("Cd", Element::Cd)
            .value("In", Element::In)
            .value("Sn", Element::Sn)
            .value("Sb", Element::Sb)
            .value("Te", Element::Te)
            .value("I", Element::I)
            .value("Xe", Element::Xe)
            .value("Cs", Element::Cs)
            .value("Ba", Element::Ba)
            .value("La", Element::La)
            .value("Ce", Element::Ce)
            .value("Pr", Element::Pr)
            .value("Nd", Element::Nd)
            .value("Pm", Element::Pm)
            .value("Sm", Element::Sm)
            .value("Eu", Element::Eu)
            .value("Gd", Element::Gd)
            .value("Tb", Element::Tb)
            .value("Dy", Element::Dy)
            .value("Ho", Element::Ho)
            .value("Er", Element::Er)
            .value("Tm", Element::Tm)
            .value("Yb", Element::Yb)
            .value("Lu", Element::Lu)
            .value("Hf", Element::Hf)
            .value("Ta", Element::Ta)
            .value("W", Element::W)
            .value("Re", Element::Re)
            .value("Os", Element::Os)
            .value("Ir", Element::Ir)
            .value("Pt", Element::Pt)
            .value("Au", Element::Au)
            .value("Hg", Element::Hg)
            .value("Tl", Element::Tl)
            .value("Pb", Element::Pb)
            .value("Bi", Element::Bi)
            .value("Po", Element::Po)
            .value("At", Element::At)
            .value("Rn", Element::Rn)
            .value("Fr", Element::Fr)
            .value("Ra", Element::Ra)
            .value("Ac", Element::Ac)
            .value("Th", Element::Th)
            .value("Pa", Element::Pa)
            .value("U", Element::U);

        py::enum_<Composite>(db, "Composite")
            .value("Adipose_Tissue", Composite::Adipose_Tissue)
            .value("Air", Composite::Air)
            .value("Air_Equivalent_Plastic", Composite::Air_Equivalent_Plastic)
            .value("Alanine", Composite::Alanine)
            .value("Ammonium_Cerium_Sulfate_Solution", Composite::Ammonium_Cerium_Sulfate_Solution)
            .value("Bakelite", Composite::Bakelite)
            .value("Blood", Composite::Blood)
            .value("Bone_Cortical", Composite::Bone_Cortical)
            .value("Bone_Equivalent_Plastic", Composite::Bone_Equivalent_Plastic)
            .value("Brain", Composite::Brain)
            .value("Breast", Composite::Breast)
            .value("Cadmium_Telluride", Composite::Cadmium_Telluride)
            .value("Calcium_Fluoride", Composite::Calcium_Fluoride)
            .value("Calcium_Sulfate", Composite::Calcium_Sulfate)
            .value("Cesium_Iodide", Composite::Cesium_Iodide)
            .value("Concrete_Barite", Composite::Concrete_Barite)
            .value("Concrete_Ordinary", Composite::Concrete_Ordinary)
            .value("Eye_Lens", Composite::Eye_Lens)
            .value("Ferrous_Sulfate", Composite::Ferrous_Sulfate)
            .value("Gadolinium_Oxysulfide", Composite::Gadolinium_Oxysulfide)
            .value("Gafchromic_Sensor", Composite::Gafchromic_Sensor)
            .value("Gallium_Arsenide", Composite::Gallium_Arsenide)
            .value("Glass_Borosilicate", Composite::Glass_Borosilicate)
            .value("Glass_Lead", Composite::Glass_Lead)
            .value("Lithium_Fluoride", Composite::Lithium_Fluoride)
            .value("Lithium_Tetraborate", Composite::Lithium_Tetraborate)
            .value("Lung", Composite::Lung)
            .value("Magnesium_Tetraborate", Composite::Magnesium_Tetraborate)
            .value("Mercuric_Iodide", Composite::Mercuric_Iodide)
            .value("Muscle_Skeletal", Composite::Muscle_Skeletal)
            .value("Ovary", Composite::Ovary)
            .value("Photographic_Emulsion_Kodak_Type_AA", Composite::Photographic_Emulsion_Kodak_Type_AA)
            .value("Photographic_Emulsion_Standard_Nuclear", Composite::Photographic_Emulsion_Standard_Nuclear)
            .value("Plastic_Scintillator_Vinyltoluene", Composite::Plastic_Scintillator_Vinyltoluene)
            .value("Polyethylene", Composite::Polyethylene)
            .value("Polyethylene_Terephthalate", Composite::Polyethylene_Terephthalate)
            .value("Polymethyl_Methacrylate", Composite::Polymethyl_Methacrylate)
            .value("Polystyrene", Composite::Polystyrene)
            .value("PVC", Composite::PVC)
            .value("Radiochromic_Dye_Film", Composite::Radiochromic_Dye_Film)
            .value("Teflon", Composite::Teflon)
            .value("Testis", Composite::Testis)
            .value("Tissue_Equivalent_Gas_Methane_Based", Composite::Tissue_Equivalent_Gas_Methane_Based)
            .value("Tissue_Equivalent_Gas_Propane_Based", Composite::Tissue_Equivalent_Gas_Propane_Based)
            .value("Tissue_Equivalent_Plastic", Composite::Tissue_Equivalent_Plastic)
            .value("Tissue_Soft", Composite::Tissue_Soft)
            .value("Tissue_Soft_Four_Component", Composite::Tissue_Soft_Four_Component)
            .value("Water", Composite::Water);

        py::enum_<Spectrum>(db, "Spectrum")
            .value("Example", Spectrum::Example);

        db.def("attenuation_model", static_cast<std::shared_ptr<AbstractIntegrableDataModel>(*)(Composite)>(&attenuationModel), "composite"_a);
        db.def("attenuation_model", static_cast<std::shared_ptr<AbstractIntegrableDataModel>(*)(Element)>(&attenuationModel), "element"_a);

        db.def("density", static_cast<float(*)(Composite)>(&density), "composite"_a);
        db.def("density", static_cast<float(*)(Element)>(&density), "element"_a);
    }

    py::class_<CTLDatabaseHandler>(m, "CTLDatabaseHandler")
        .def_static("set_database_root", [](const std::string& path)
        {
            CTLDatabaseHandler::instance().setDatabaseRoot(QString::fromStdString(path));
        }, "path"_a)
        .def_static("load_attenuation_model", [](database::Composite c)
        {
            return CTLDatabaseHandler::instance().loadAttenuationModel(c);
        }, "composite"_a)
        .def_static("load_attenuation_model", [](database::Element e)
        {
            return CTLDatabaseHandler::instance().loadAttenuationModel(e);
        }, "element"_a)
        .def_static("load_density", [](database::Composite c)
        {
            return CTLDatabaseHandler::instance().loadDensity(c);
        }, "composite"_a)
        .def_static("load_density", [](database::Element e)
        {
            return CTLDatabaseHandler::instance().loadDensity(e);
        }, "composite"_a);
}
