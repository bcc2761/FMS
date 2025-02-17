!***********************************************************************
!*                   GNU Lesser General Public License
!*
!* This file is part of the GFDL Flexible Modeling System (FMS).
!*
!* FMS is free software: you can redistribute it and/or modify it under
!* the terms of the GNU Lesser General Public License as published by
!* the Free Software Foundation, either version 3 of the License, or (at
!* your option) any later version.
!*
!* FMS is distributed in the hope that it will be useful, but WITHOUT
!* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
!* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
!* for more details.
!*
!* You should have received a copy of the GNU Lesser General Public
!* License along with FMS.  If not, see <http://www.gnu.org/licenses/>.
!***********************************************************************

!> This file contains subroutines that initialize data and write out the climatology file.

subroutine write_climatology_file

  implicit none

  type(FmsNetcdfFile_t) :: fileobj
  integer :: i,j,k,l, itime
  integer :: stringlen
  character(100) :: mystring

  ! write netcdf file
  if(open_file(fileobj, 'INPUT/'//trim(ncfile), 'overwrite')) then
     call register_axis(fileobj, 'time', UNLIMITED)
     call register_axis(fileobj, 'lon',   nlonlat)
     call register_axis(fileobj, 'lat',   nlonlat)
     call register_axis(fileobj, 'latb',  nlonlatb)
     call register_axis(fileobj, 'lonb',  nlonlatb)
     call register_axis(fileobj, 'pfull', npfull)
     call register_axis(fileobj, 'phalf', nphalf)
     call register_axis(fileobj, 'longitude', nlonlat)
     call register_axis(fileobj, 'latitude',  nlonlat)

     !time axes
     !if(ntime /=0 ) then
     call register_field(fileobj, 'time', 'double', dimensions=(/'time'/))
     call register_string_attribute(fileobj, 'time', 'axis', 'T')
     call register_string_attribute(fileobj, 'time', 'calendar', 'noleap')
     call register_string_attribute(fileobj, 'time', 'climatology', '1979-01-01 00:00:00, 1998-01-01 00:00:00')
     call register_string_attribute(fileobj, 'time', 'units', 'days since 1849-01-01 00:00:00')
     !end if

     !lon axes
     call register_field(fileobj, 'lon', 'double', dimensions=(/'longitude'/))
     call register_string_attribute(fileobj, 'lon', 'edges', 'lonb')
     call register_string_attribute(fileobj, 'lon', 'cartesian_axis', 'X')
     call register_string_attribute(fileobj, 'lon', 'units', 'degrees_east')

     !lonb  axes longitude edges
     call register_field(fileobj, 'lonb', 'double', dimensions=(/'lonb'/))
     call register_string_attribute(fileobj, 'lonb', 'cartesian_axis','X')
     call register_string_attribute(fileobj, 'lonb', 'units', 'degrees_edges')

     !lat
     call register_field(fileobj, 'lat', 'double', dimensions=(/'latitude'/))
     call register_string_attribute(fileobj, 'lat', 'edges', 'latb')
     call register_string_attribute(fileobj, 'lat', 'cartesian_axis', 'Y')
     call register_string_attribute(fileobj, 'lat', 'units', 'degrees_north')

     !latb latitude_edges
     call register_field(fileobj, 'latb', 'double', dimensions=(/'latb'/))
     call register_string_attribute(fileobj, 'latb', 'cartesian_axis', 'Y')
     call register_string_attribute(fileobj, 'latb', 'units', 'degrees_N')

     !pfull layer-mean pressure
     call register_field(fileobj, 'pfull', 'double', dimensions=(/'pfull'/))
     call register_string_attribute(fileobj, 'pfull', 'positive', 'down')
     call register_string_attribute(fileobj, 'pfull', 'cartesian_axis', 'Z')
     call register_string_attribute(fileobj, 'pfull', 'units', 'hPa')

     !phalf half pressure level
     call register_field(fileobj, 'phalf', 'double', dimensions=(/'phalf'/))
     call register_string_attribute(fileobj, 'phalf', 'positive', 'down')
     call register_string_attribute(fileobj, 'phalf', 'cartesian_axis', 'Z')
     call register_string_attribute(fileobj, 'phalf', 'units', 'hPa')

     !ozone ozone mass mixing ratio
     if( ntime==0 ) then
        call register_field(fileobj, 'ozone', 'double', dimensions=(/'lon  ','lat  ','pfull'/))
     else
        call register_field(fileobj, 'ozone', 'double', dimensions=(/'lon  ','lat  ','pfull','time '/))
     end if
     call register_string_attribute(fileobj, 'ozone', 'units', 'kg/kg')

     if(ntime /= 0 ) call write_data(fileobj, 'time',  clim_time)
     call write_data(fileobj, 'lon',   lon)
     call write_data(fileobj, 'lonb',  lonb)
     call write_data(fileobj, 'lat',   lat)
     call write_data(fileobj, 'latb',  latb)
     call write_data(fileobj, 'pfull', pfull)
     call write_data(fileobj, 'phalf', phalf)
     call write_data(fileobj, 'ozone', ozone)

     call close_file(fileobj)

  end if

end subroutine write_climatology_file
!===============================================!
subroutine register_string_attribute(fileobj, variable, attribute, att_value)

  implicit none

  type(FmsNetcdfFile_t) :: fileobj
  character(*) :: variable, attribute, att_value
  integer :: str_len

  str_len=len(trim(att_value))
  call register_variable_attribute(fileobj, trim(variable), trim(attribute), trim(att_value), str_len=str_len)

end subroutine register_string_attribute
!===============================================!
subroutine allocate_arrays()

  implicit none
  integer :: ntime2

  ntime2=ntime
  if(ntime==0) ntime2=1

  allocate( lat(nlonlat),     &
            lon(nlonlat),     &
            latb(nlonlatb),   &
            lonb(nlonlatb),   &
            clim_time(ntime2),&
            pfull(npfull),    &
            phalf(nphalf),    &
            ozone(nlonlat, nlonlat, npfull, ntime2) )

  allocate( lat_mod(nlonlat_mod, nlonlat_mod),    &
            lon_mod(nlonlat_mod, nlonlat_mod),    &
            latb_mod(nlonlatb_mod, nlonlatb_mod), &
            lonb_mod(nlonlatb_mod, nlonlatb_mod) )

end subroutine allocate_arrays
!===============================================!
subroutine deallocate_arrays()

  implicit none
  deallocate( lat,   &
              lon,   &
              latb,  &
              lonb,  &
              clim_time,  &
              pfull, &
              phalf, &
              lat_mod,   &
              lon_mod,   &
              latb_mod,  &
              lonb_mod,  &
              ozone )

end subroutine deallocate_arrays
!===============================================!
subroutine set_write_data(nlonlat_in, nlonlat_mod_in, ntime_in, npfull_in)

  implicit none
  integer, intent(in) :: nlonlat_in, nlonlat_mod_in, ntime_in, npfull_in

  nlonlat  = nlonlat_in   !< number of latitude and longitudinal center coordinates
  nlonlatb = nlonlat_in+1 !< number of latitude and longitudinal boundary coordinates
  nlonlat_mod  = nlonlat_mod_in
  nlonlatb_mod = nlonlat_mod_in+1
  ntime  = ntime_in    !< number of time slices
  npfull = npfull_in   !< number of p levels
  nphalf = npfull_in+1 !< number of half p levels
  call allocate_arrays()
  if( ntime /= 0) call set_clim_time()
  call set_latlon_b()
  call set_pfullhalf()
  call set_ozone()
  call write_climatology_file()

  call set_latlon_b_mod()

end subroutine set_write_data
!===============================================!
subroutine set_clim_time()

  implicit none
  integer :: i

  do i=1, ntime
     clim_time(i)=2*i
  end do

end subroutine set_clim_time
!===============================================!
subroutine set_latlon_b()

  implicit none
  integer :: i

  do i=1, nlonlat
     lat(i)= real(2*i-1,TEST_INTP_KIND_)
     lon(i)= real(2*i-1,TEST_INTP_KIND_)
  end do

  lonb(1)=1.0_lkind
  latb(1)=1.0_lkind
  do i=2,nlonlat
     lonb(i)=0.5_lkind*(lon(i-1)+lon(i))
     latb(i)=0.5_lkind*(lat(i-1)+lat(i))
  enddo
  lonb(nlonlat+1)=real(2*nlonlat-1)
  latb(nlonlat+1)=real(2*nlonlat-1)

end subroutine set_latlon_b
!===============================================!
!> create model lat(b) and lon(b) arrays
subroutine set_latlon_b_mod

  implicit none
  integer :: i, j

  !> the model grid and the climatology grid are the same
  do i=1, nlonlat
     lon_mod(:,i) = real(2*i-1,TEST_INTP_KIND_)
     lat_mod(i,:) = real(2*i-1,TEST_INTP_KIND_)
  end do

  lonb_mod(1:nlonlatb,1)=1.0_lkind
  latb_mod(1,1:nlonlatb)=1.0_lkind
  do i=1, nlonlat-1
     lonb_mod(1:nlonlatb,i+1)=2*i
     latb_mod(i+1,1:nlonlatb)=2*i
  end do
  lonb_mod(1:nlonlatb,nlonlatb)=real(2*nlonlat-1,TEST_INTP_KIND_)
  latb_mod(nlonlatb,1:nlonlatb)=real(2*nlonlat-1,TEST_INTP_KIND_)

  !> convert from degrees to radians
  lon_mod  = lon_mod*real(PI,TEST_INTP_KIND_)/180.0_lkind
  lat_mod  = lat_mod*real(PI,TEST_INTP_KIND_)/180.0_lkind
  lonb_mod = lonb_mod*real(PI,TEST_INTP_KIND_)/180.0_lkind
  latb_mod = latb_mod*real(PI,TEST_INTP_KIND_)/180.0_lkind


end subroutine set_latlon_b_mod
!===============================================!
subroutine set_pfullhalf()

  implicit none
  integer :: i

  do i=1, npfull
     pfull(i) = 0.0001_lkind * real(i-1,TEST_INTP_KIND_)
  end do

  phalf(1)=0.0_lkind
  do i=2, npfull
     phalf(i)=0.5_lkind*( pfull(i)+pfull(i-1) )
  end do
  phalf(npfull+1)=pfull(npfull)

end subroutine set_pfullhalf
!===============================================!
subroutine set_ozone()

  implicit none
  integer :: i, j, k, l
  integer :: ntime2

  ntime2=ntime
  if(ntime == 0 ) ntime2=1

  do i=1, ntime2
     do j=1, npfull
        do k=1, nlonlat
           do l=1, nlonlat
              ozone(l,k,j,i)=real(i,TEST_INTP_KIND_)
           end do
        end do
     end do
  end do

end subroutine set_ozone
!===============================================!
