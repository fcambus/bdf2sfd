Name:           bdf2sfd
Version:        1.1.8
Release:        1%{?dist}
Summary:        BDF to SFD converter

License:        BSD-2-Clause
URL:            https://github.com/fcambus/bdf2sfd
Source0:        %{url}/releases/download/%{version}/%{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc

%description
bdf2sfd is a BDF to SFD converter, allowing to vectorize bitmap fonts.

It works by converting each pixel of a glyph to a polygon, which produces
large and unoptimized SFD files that should be post-processed using FontForge.

%prep
%autosetup

%build
%cmake .
%cmake_build

%install
%cmake_install

%check

%files
%{_bindir}/%{name}
%{_mandir}/man1/%{name}.1*
%license LICENSE
%doc README.md
