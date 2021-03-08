#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <acquisition/viewgeometry.h>

namespace py = pybind11;

namespace
{
std::vector<double> svg2vec(const CTL::SingleViewGeometry& svg)
{
    std::vector<double> ret;
    ret.reserve(svg.nbModules()*12u);

    for(const auto& pmat : svg)
        for(const auto el : pmat)
            ret.push_back(el);
    
    return ret;
}
}

void init_viewgeometry(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;
    
    py::class_<SingleViewGeometry>(m, "SingleViewGeometry")
        .def(py::init<>())
        .def(py::init<uint>(), "nb_modules"_a)
        .def(py::init([](const std::vector<mat::ProjectionMatrix>& pMats)
        {
            // TODO: don't use fromStdVector
            return SingleViewGeometry(QVector<mat::ProjectionMatrix>::fromStdVector(pMats));
        }), "p_mats"_a)
        .def("append", [](SingleViewGeometry& self, const mat::ProjectionMatrix& pMat)
        {
            self.append(pMat);
        }, "p_mat")
        .def("append", [](SingleViewGeometry& self, const std::vector<mat::ProjectionMatrix>& pMats)
        {
            // TODO: don't use fromStdVector
            self.append(QVector<mat::ProjectionMatrix>::fromStdVector(pMats));
        }, "p_mat")
        .def("append", [](SingleViewGeometry& self, const SingleViewGeometry& other)
        {
            self.append(other);
        }, "other"_a)
        .def("clear", &SingleViewGeometry::clear)
        .def("concatenated", &SingleViewGeometry::concatenatedStdVector)
        .def("__len__", &SingleViewGeometry::length)
        .def("module", &SingleViewGeometry::module, "i"_a)
        .def("nb_modules", &SingleViewGeometry::nbModules)
        .def("reserve", &SingleViewGeometry::reserve, "nb_modules"_a)
        .def("__getitem__", [](SingleViewGeometry& self, uint i)
        {
            return self[i];
        }, "i"_a, py::keep_alive<0,1>())
        .def("__setitem__", [](SingleViewGeometry& self,
                               uint key,
                               const mat::ProjectionMatrix& pMat)
        {
            self[key] = pMat;
        }, "key"_a, "value"_a)
        .def("__iter__", [](SingleViewGeometry& self)
        {
            return py::make_iterator(self.begin(), self.end());
        }, py::keep_alive<0,1>())
        .def_static("from_numpy", [](py::array_t<double,py::array::c_style|py::array::forcecast> array)
        {
            const auto info { array.request() };
            const auto nbModules { static_cast<uint>(info.shape[0]) };
            const auto height { static_cast<uint>(info.shape[1]) };
            const auto width { static_cast<uint>(info.shape[2]) };
            const auto fptr { static_cast<double*>(info.ptr) };

            if (width != 4 || height != 3)
            {
                throw std::invalid_argument("array.shape must be [nb_modules, 3, 4]");
            }

            QVector<mat::ProjectionMatrix> pMats;
            for (auto module { 0u }; module < nbModules; module++)
            {
                pMats.push_back(mat::ProjectionMatrix::fromContainer(
                    std::vector<double>{
                        fptr + 12*module,
                        fptr + 12*module + 12 }, 0));
            }

            return SingleViewGeometry{std::move(pMats)};
        }, "array"_a)
        .def("numpy", [](const SingleViewGeometry& self) -> py::array_t<double>
        {
            return {
                { self.nbModules(), 3u, 4u },
                { sizeof(double)*12, sizeof(double)*4, sizeof(double) },
                svg2vec(self).data()
            };
        }, "Copies the internal data into a new NumPy array of shape [nb_modules, 3, 4]");

    py::class_<FullGeometry>(m, "FullGeometry")
        .def(py::init<>())
        .def(py::init<uint>(), "nb_views"_a)
        .def(py::init([](const std::vector<SingleViewGeometry>& pMats)
        {
            // TODO: don't use fromStdVector
            return FullGeometry(QVector<SingleViewGeometry>::fromStdVector(pMats));
        }), "p_mats"_a)
        .def("append", [](FullGeometry& self, const SingleViewGeometry& pMat)
        {
            self.append(pMat);
        }, "p_mat")
        .def("append", [](FullGeometry& self, const std::vector<SingleViewGeometry>& pMats)
        {
            // TODO: don't use fromStdVector
            self.append(QVector<SingleViewGeometry>::fromStdVector(pMats));
        }, "p_mat")
        .def("append", [](FullGeometry& self, const FullGeometry& other)
        {
            self.append(other);
        }, "other"_a)
        .def("clear", &FullGeometry::clear)
        .def("concatenated", &FullGeometry::concatenatedStdVector)
        .def("__len__", &FullGeometry::length)
        .def("nb_view", &FullGeometry::nbViews)
        .def("reserve", &FullGeometry::reserve, "nb_views"_a)
        .def("__getitem__", [](FullGeometry& self, uint i)
        {
            return self[i];
        }, "i"_a, py::keep_alive<0,1>())
        .def("__setitem__", [](SingleViewGeometry& self,
                               uint key,
                               const mat::ProjectionMatrix& pMat)
        {
            self[key] = pMat;
        }, "key"_a, "value"_a)
        .def("__iter__", [](FullGeometry& self)
        {
            return py::make_iterator(self.begin(), self.end());
        }, py::keep_alive<0,1>())
        .def_static("from_numpy", [](py::array_t<double,py::array::c_style|py::array::forcecast> array)
        {
            const auto info { array.request() };
            const auto nbViews { static_cast<uint>(info.shape[0]) };
            const auto nbModules { static_cast<uint>(info.shape[1]) };
            const auto height { static_cast<uint>(info.shape[2]) };
            const auto width { static_cast<uint>(info.shape[3]) };
            const auto fptr { static_cast<double*>(info.ptr) };

            if (width != 4 || height != 3)
            {
                throw std::invalid_argument("array.shape must be [nb_views, nb_modules, 3, 4]");
            }

            QVector<SingleViewGeometry> views;
            for (auto view { 0u }; view < nbViews; view++)
            {
                QVector<mat::ProjectionMatrix> pMats;
                for (auto mod { 0u }; mod < nbModules; mod++)
                {
                    pMats.push_back(mat::ProjectionMatrix::fromContainer(
                        std::vector<double>{
                            fptr + nbModules*12*view + 12*mod,
                            fptr + nbModules*12*view + 12*mod + 12 }, 0));
                }
                views.push_back(SingleViewGeometry{std::move(pMats)});
            }

            return FullGeometry{std::move(views)};
        }, "array"_a)
        .def("numpy", [](const FullGeometry& self) -> py::array_t<double>
        {
            std::vector<double> ret;

            const auto nbViews { self.nbViews() };
            const auto nbModules { self.view(0).nbModules() };
            ret.reserve(nbViews * nbModules * 12u);

            for(const auto& view : self)
                for(const auto viewPMat : svg2vec(view))
                    ret.push_back(viewPMat);

            return {
                { nbViews, nbModules, 3u, 4u },
                { sizeof(double)*nbModules*12, sizeof(double)*12, sizeof(double)*4, sizeof(double) },
                ret.data()
            };
        }, "Copies the internal data into a new NumPy array of shape [nb_views, nb_modules, 3, 4]");
}