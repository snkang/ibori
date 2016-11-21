#include <QCoreApplication>
#include <stdio.h>
#include <pcap.h>

#include <netinet/in.h>
#include "libs/config.h"
#include "libs/libnet-macros.h"
#include "libs/libnet-headers.h"

#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <time.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2 ) {
        printf("ibory <interface>\n");
        return(0);
    }

    char *dev = argv[1];
    printf("Device: %s\n", dev);

    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];          /* Error string */
    struct bpf_program fp;                  /* The compiled filter expression */
    char filter_exp[] = "tcp port 80";      /* The filter expression */
    bpf_u_int32 mask;                       /* The netmask of our sniffing device */
    bpf_u_int32 net;                        /* The IP of our sniffing device */
    struct pcap_pkthdr header;              /* The header that pcap gives us */
    const u_char *packet;                   /* The actual packet */
    const int WORD_SIZE = 4;                /* "GET"/"POST" variable size */
    char str1[WORD_SIZE] = "";              /* "GET"/"POST" variable */

    boost::smatch m;                        /* Matches */
    boost::regex pat;
    boost::regex URLs( ".*((?<Protocol>http|ftp|file)\:\/\/(?<Domain>[\\w\@][\\w\.\:\@]+)(\/\?[\\w\\.?=%&-@/$,]*)).*" );  // m[1]
    boost::regex Cookie( ".*Cookie\: ((\\w+\=[\\w\\d]+)\;\\s([\\w\\d?=%&-@/$,]*)).*" );         // m[1]
    boost::regex User_ID( ".*\<user_id\>\<\!\\[\\w+\\[(\.+)\\]\\]\>\<\/user_id\>.*" );          // m[1]
    boost::regex Password( ".*\<password\>\<\!\\[\\w+\\[(\.+)\\]\\]\>\<\/password\>.*" );       // m[1]

    uint numKeywords = 1;

    // ----------------  Packet Capture -----------------------
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return(2);
    }

    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return(2);
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return(2);
    }

    while(true) {
        /* Grab a packet */
        packet = pcap_next(handle, &header);
        if (packet == NULL) continue;

        libnet_ethernet_hdr* eth_hdr = (libnet_ethernet_hdr*) packet;
        if ( ntohs(eth_hdr->ether_type) != ETHERTYPE_IP /* 0x0800 */ )
            continue;

        libnet_ipv4_hdr* ip_hdr = (libnet_ipv4_hdr*) ((char*)eth_hdr + 14 /* Ethernet MAC header size */);
        if ( ip_hdr->ip_p != 6 /* tcp */)
            continue;

        libnet_tcp_hdr* tcp_hdr = (libnet_tcp_hdr*) ((char*)ip_hdr + (ip_hdr->ip_hl * 4));
        u_int16_t sport = ntohs(tcp_hdr->th_sport);
        u_int16_t dport = ntohs(tcp_hdr->th_dport);

        /* Print Port number*/
        //        printf("%u->%u\n", sport, dport);

        /* Print its length */
        //        printf("Jacked packet length : [%d]", header.len);

        u_int16_t tcp_seg_len = ntohs(ip_hdr->ip_len) - (u_int16_t)(ip_hdr->ip_hl * 4 + tcp_hdr->th_off * 4);

        /* Print tcp segment's length */
        //        printf("\n IP Header Total length : [%d], Fragmented : [%d]", ntohs(ip_hdr->ip_len), ip_hdr->ip_off);
        //        printf("\n IP Header length : [%d]", ip_hdr->ip_hl*4);
        //        printf("\n TCP Header length : [%d]", tcp_hdr->th_off*4);
        //        printf("\n Tcp segment's length : [%d]\n", tcp_seg_len);

        if (tcp_seg_len <= 0) {
            continue;               /* No TCP segments  */
        }

        char* tcp_segment = (char*)((char*)tcp_hdr + tcp_hdr->th_off * 4);

        /* Print TCP segment */
        //            printf("%s\n", tcp_segment);


        /* Filter Http request packets only. */
        strncpy ( str1, tcp_segment, sizeof(char)*3 );
        //            cout << "* str1 : " << str1 << endl;

        if ( strcmp(str1, "GET")*strcmp(str1, "POS") != 0 ) {
//            cout << "GET/POST not matched!!" << endl;
            continue;
        }
        cout << "HTTP request (GET/POST) packet!!" << endl;


        /* Find search Keywords */
        string buffer = string(tcp_segment);

        /* Search URL */
        pat = URLs;
        if (boost::regex_match(buffer, m, pat)) {
           cout << "# URL : " << m[1] << endl;
        }

        /* Search Cookie */
        pat = Cookie;
        if (boost::regex_match(buffer, m, pat)) {
            cout << "# Cookie : " << m[1] << endl;
        }

        /* Search USER_ID */
        pat = User_ID;
        if (boost::regex_match(buffer, m, pat)) {
            cout << "# User_ID : " << m[1] << endl;
        }

        /* Search PASSWORD */
        pat = Password;
        if (boost::regex_match(buffer, m, pat)) {
            cout << "# Password : " << m[1] << endl;
        }

        /* Get Date(ISO format) */
        time_t secs=time(0);
        tm *t=localtime(&secs);
        cout << "# Date : " << t->tm_year+1900 << "-" << t->tm_mon+1 << "-" << t->tm_mday << " " <<
                t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << endl;
    }

    /* And close the session */
    pcap_close(handle);

    QCoreApplication a(argc, argv);
    return a.exec();

    return(0);
}
