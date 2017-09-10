%define debug_package %{nil}
%define _sourcedir %(pwd)
%define _topdir %(pwd)
%define _builddir %(pwd)
%define _buildrootdir %(pwd)
%define _rpmdir %(pwd)
%define service_name slarht

Name:           %{service_name}
Version:        %{git_tag}
Release:        0%{?dist}
Summary:        Simple Lightweight Artefacts Restful HTtp repository
Group:          Applications/Internet
License:        Apache licence 2 version
BuildRequires:  gcc libevent libevent-devel
Requires:	nginx
Source:		slarht.service
AutoReq: no

%description
Simple Lightweight Artefacts Restful HTtp repository

%build
cd %{_sourcedir}
cc -levent evhttp.c -o slarht

%install
%{__mkdir_p} ${RPM_BUILD_ROOT}/usr/bin/
%{__mkdir_p} $RPM_BUILD_ROOT%{_unitdir}
%{__cp} slarht ${RPM_BUILD_ROOT}/usr/bin/
%{__cp} %{SOURCE1} $RPM_BUILD_ROOT%{_unitdir}

%files
/usr/bin/%{service_name}
%{_unitdir}/%{service_name}.service

%post
if [ "$1" = "1" ]; then
    getent passwd %{service_user} >/dev/null || \
        useradd -m -d /var/lib/%{service_user} -c "slarht" %{service_user} || :
fi

%{__chown} -R %{service_user}:%{service_user} %{_localstatedir}/log/%{service_name}
%{__chown} -R %{service_user}:%{service_user} /spool/%{service_name}
%systemd_post %{service_name}.service

%preun
%systemd_preun %{service_name}.service

%postun
%systemd_postun %{service_name}.service

%changelog
