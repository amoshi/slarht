%define debug_package %{nil}
%define _sourcedir %(pwd)
%define _topdir %(pwd)
%define service_name slarth

Name:           %{service_name}
Version:        %{git_tag}
Release:        0%{?dist}
Summary:        Simple Lightweight Artefacts Restful HTtp repository
Group:          Applications/Internet
License:        Apache licence 2 version
BuildRequires:  gcc libevent libevent-devel
Requires:	nginx
AutoReq: no

%description
Simple Lightweight Artefacts Restful HTtp repository

%build
cd %{_sourcedir}
cc -levent evhttp.c -o slarht

%install
%{__mkdir_p} ${RPM_BUILD_ROOT}/usr/bin/
cp -a slarht ${RPM_BUILD_ROOT}/usr/bin/

%files
/usr/bin/%{service_name}

%changelog
