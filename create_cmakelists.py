import os

def extract_from_qmake(path, variable):
    sources_list = []

    def next_linelist(it):
        line = next(it)
        line = line.replace('\n', '').strip()
        return [tok for tok in line.split() if tok != '']

    with open(path, 'r') as qfile:
        line_iter = iter(qfile)
        try:
            while True:
                linelist = next_linelist(line_iter)
                if len(linelist) > 1 and ''.join(linelist[0:2]) == variable + '+=':
                    continue_line = linelist[-1] == '\\'
                    sources_list += linelist[2:-1] if continue_line else linelist[2:]
                    while continue_line:
                        linelist = next_linelist(line_iter)
                        continue_line = linelist[-1] == '\\'
                        sources_list += linelist[:-1] if continue_line else linelist
        except StopIteration:
            pass
    return sources_list

def insert_ctl_sources(file_handle, ctl_dir):
    extract_sources_from_qmake = lambda x: extract_from_qmake(x, 'SOURCES')
    qt_cpp_files = \
        extract_sources_from_qmake(ctl_dir + '/modules/submodules/ctl_core.pri') + \
        extract_sources_from_qmake(ctl_dir + '/modules/submodules/ocl_config.pri') + \
        extract_sources_from_qmake(ctl_dir + '/modules/submodules/ocl_routines.pri') + \
        extract_sources_from_qmake(ctl_dir + '/modules/submodules/gui_widgets.pri') + \
        extract_sources_from_qmake(ctl_dir + '/modules/submodules/gui_widgets_3d.pri') + \
        extract_sources_from_qmake(ctl_dir + '/modules/submodules/gui_widgets_ocl.pri')

    cmake_cpp_files = [f.replace('$$PWD/../src', '${CTL_DIR}') for f in qt_cpp_files]
    file_handle.writelines([' '*4 + f + '\n' for f in cmake_cpp_files])

def insert_ctl_forms(file_handle, ctl_dir):
    extract_forms_from_qmake = lambda x: extract_from_qmake(x, 'FORMS')
    qt_cpp_files = \
        extract_forms_from_qmake(ctl_dir + '/modules/submodules/ctl_core.pri') + \
        extract_forms_from_qmake(ctl_dir + '/modules/submodules/ocl_config.pri') + \
        extract_forms_from_qmake(ctl_dir + '/modules/submodules/ocl_routines.pri') + \
        extract_forms_from_qmake(ctl_dir + '/modules/submodules/gui_widgets.pri') + \
        extract_forms_from_qmake(ctl_dir + '/modules/submodules/gui_widgets_3d.pri') + \
        extract_forms_from_qmake(ctl_dir + '/modules/submodules/gui_widgets_ocl.pri')

    cmake_cpp_files = [f.replace('$$PWD/../src', '${CTL_DIR}') for f in qt_cpp_files]
    file_handle.writelines([' '*4 + f + '\n' for f in cmake_cpp_files])

def insert_ctl_src_dir(file_handle, ctl_dir):
    file_handle.write(' '*4 + '/'.join([ctl_dir, 'modules', 'src']) + '\n')

def insert_pyctl_sources(file_handle):
    cpp_files = [' '*4 + '/'.join([r.replace('\\', '/'), f]) + '\n'
                 for r, _, fl in os.walk('pysrc')
                 for f in fl if not f.endswith('.h')]
    file_handle.writelines(cpp_files)

def writeline(l):
    def w(h):
        h.write(l + '\n')
    return w

def create_cmakelists(ctl_dir):
    ctl_dir = ctl_dir[:-1] if ctl_dir.endswith('/') else ctl_dir
    with open('CMakeListsTemplate.txt', 'r') as template_handle, \
         open('CMakeLists.txt', 'w') as generated_handle:
        for template_line in [l.replace('\n', '') for l in template_handle]:
            {
                '$$insert_ctl_dir$$': lambda x: insert_ctl_src_dir(x, ctl_dir),
                '$$insert_ctl_sources$$': lambda x: insert_ctl_sources(x, ctl_dir),
                '$$insert_ctl_forms$$': lambda x: insert_ctl_forms(x, ctl_dir),
                '$$insert_pyctl_sources$$': insert_pyctl_sources,
            }.get(template_line, writeline(template_line))(generated_handle)

if __name__ == '__main__':
    create_cmakelists('ctl_src')
