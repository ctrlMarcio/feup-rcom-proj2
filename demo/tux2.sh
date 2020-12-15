# sets the IP
ifconfig eth0 down
ifconfig eth0 up
ifconfig eth0 172.16.31.1/24

# adds the route to vlan0 through tux4
route add -net 172.16.30.0/24 gw 172.16.31.253

# sets the rc router as the default route
route add default gw 172.16.31.254