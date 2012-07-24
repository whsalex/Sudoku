#
# spec file for package sudoku (Version 1.0)
#
# Please submit bugfixes or comments via http:///

Name:           sudoku
License:        GNU General Public License (GPL)
Group:          Amusements/Games
Summary:        A solution of sudoku
#Requires:       strongwind ctcs2
Version:        1.0
Release:        2
Source0:        %{name}-%{version}.tar.bz2
Source1:	sudoku.6
Patch:		%{name}-%{version}.patch
BuildRoot:      %{_topdir}/%{_tmpfloder}/%{name}-%{version}-build
BuildArch:      noarch

%description
Solution of Sudoku.

%prep
%setup -q
%patch -p1

%build
make

%install
install -m 755 -d $RPM_BUILD_ROOT/usr/share/man/man6
install -m 644 %{SOURCE1} $RPM_BUILD_ROOT/usr/share/man/man6
gzip $RPM_BUILD_ROOT/usr/share/man/man6/%{name}.6
#install -m 755 -d $RPM_BUILD_ROOT/usr/share/Sudoku
#install -m 755 shuduku $RPM_BUILD_ROOT/usr/share/Sudoku/
install -m 755 -d $RPM_BUILD_ROOT/usr/bin
install -m 755 shuduku $RPM_BUILD_ROOT/usr/bin/
install -m 755 -d $RPM_BUILD_ROOT/etc/Sudoku
install -m 644 releasenote $RPM_BUILD_ROOT/etc/Sudoku/
install -m 644 config_sdk $RPM_BUILD_ROOT/etc/Sudoku/

%clean
rm -rf $RPM_BUILD_ROOT
rm -rf $RPM_BUILD_DIR/%{name}-%{version}

%files
%defattr(-, root, root)
/usr/share/man/man6/sudoku.6.gz
/usr/bin/shuduku

%doc /etc/Sudoku/releasenote
%config /etc/Sudoku/config_sdk

%changelog
* Thu Aug 26 2011 - nwang@novell.com
- Create a patch, version 2

* Thu Aug 25 2011 - nwang@novell.com
- package created, version 1
