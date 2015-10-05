1. support NAT on gateway:

The process of iptables:
http://www.netfilter.org/documentation/HOWTO/NAT-HOWTO-5.html

iptables -t nat -A POSTROUTING -d 87.254.212.121 -j SNAT --to-source 10.56.126.85

2. echo 1 > /proc/sys/net/ipv4/ip\_forward

3. add route at source server
route add -host 87.254.212.121 gw 10.56.126.85


#################################################
4. config NAT proxy for LDAP.

iptables -t nat -A PREROUTING -p tcp -d 10.56.117.82 --dport 389 -j DNAT --to-destination 87.254.222.170:389
iptables -t nat -A PREROUTING -p udp -d 10.56.117.82 --dport 389 -j DNAT --to-destination 87.254.222.170:389
iptables -t nat -A POSTROUTING -d 87.254.222.170 -j SNAT --to-source 10.56.117.82

#################################################
5. config SVN proxy for 85.

iptables -t nat -A POSTROUTING -d 87.254.208.78 -j SNAT --to-source 10.56.126.77
route add -host 87.254.208.78 gw 10.56.126.77