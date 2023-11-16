#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define IP_BITS 32
#define MAX_OCTET_VALUE 0xFF

typedef struct {
    uint8_t octets[4];
    int prefix;
} IPAddress;

typedef struct CustomerNode {
    IPAddress startIP;
    IPAddress endIP;
    struct CustomerNode* next;
} CustomerNode;

typedef struct {
    int numCustomers;
    int numAddressesPerCustomer;
} GroupInfo;

GroupInfo getUserInputForGroup(int groupNumber) {
    GroupInfo groupInfo;
    printf("Enter the number of customers for group %d: ", groupNumber);
    scanf("%d", &groupInfo.numCustomers);
    printf("Enter the number of addresses per customer for group %d: ", groupNumber);
    scanf("%d", &groupInfo.numAddressesPerCustomer);
    return groupInfo;
}

uint32_t calculateTotalHosts(int prefix) {
    return (1u << (IP_BITS - prefix));
}

uint32_t getNextPowerOfTwo(uint32_t n) {
    uint32_t m = 0;
    while ((1u << m) < n) {
        m++;
    }
    return m;
}

void printIPAddress(IPAddress ip) {
    printf("%hhu.%hhu.%hhu.%hhu/%d\n", ip.octets[0], ip.octets[1], ip.octets[2], ip.octets[3], ip.prefix);
}

IPAddress calculateEndIPAddress(IPAddress startIP, int numAddresses) {
    int newPrefix = IP_BITS - getNextPowerOfTwo(numAddresses);
    uint32_t startAddress = (startIP.octets[0] << 24) | (startIP.octets[1] << 16) | (startIP.octets[2] << 8) | startIP.octets[3];
    uint32_t endAddress = startAddress + numAddresses - 1;
    endAddress |= (1u << (IP_BITS - newPrefix)) - 1;

    return (IPAddress) {
        .octets[0] = (endAddress >> 24) & MAX_OCTET_VALUE,
        .octets[1] = (endAddress >> 16) & MAX_OCTET_VALUE,
        .octets[2] = (endAddress >> 8) & MAX_OCTET_VALUE,
        .octets[3] = endAddress & MAX_OCTET_VALUE,
        .prefix = startIP.prefix  // Use the original startIP prefix
    };
}

void printAddressAggregation(IPAddress startIP, IPAddress endIP) {
    printf("Group IP addresses:\n");
    printf("+------------------------+\n");
    printf("| Start IP: ");
    printIPAddress(startIP);
    printf("| End IP:   ");
    printIPAddress(endIP);
    printf("+------------------------+\n");
}

CustomerNode* createCustomerNode(IPAddress startIP, IPAddress endIP) {
    CustomerNode* node = malloc(sizeof(CustomerNode));
    node->startIP = startIP;
    node->endIP = endIP;
    node->next = NULL;
    return node;
}

void printFirstAndLast(CustomerNode* head) {
    int count = 0;
    if (head == NULL) {
        return;
    }

    CustomerNode* current = head;
    printf("Customer IP addresses:\n");
    printf("Customer 1:\n");
    printf("+------------------------+\n");
    printf("| Start IP: ");
    printIPAddress(current->startIP);
    printf("| End IP:   ");
    printIPAddress(current->endIP);
    printf("+------------------------+\n");

    while (current->next != NULL) {
        count++;
        current = current->next;
    }
    printf("Customer %d:\n", count + 1);

    printf("+------------------------+\n");
    printf("| Start IP: ");
    printIPAddress(current->startIP);
    printf("| End IP:   ");
    printIPAddress(current->endIP);
    printf("+------------------------+\n");
}

IPAddress getNextIPAddress(IPAddress ip) {
    IPAddress nextIP = ip;
    nextIP.octets[3]++;

    if (nextIP.octets[3] == 0) {
        nextIP.octets[2]++;
        if (nextIP.octets[2] == 0) {
            nextIP.octets[1]++;
            if (nextIP.octets[1] == 0) {
                nextIP.octets[0]++;
            }
        }
    }

    return nextIP;
}

IPAddress intToIPAddress(uint32_t value, int prefix) {
    return (IPAddress) {
        .octets[0] = (value >> 24) & MAX_OCTET_VALUE,
        .octets[1] = (value >> 16) & MAX_OCTET_VALUE,
        .octets[2] = (value >> 8) & MAX_OCTET_VALUE,
        .octets[3] = value & MAX_OCTET_VALUE,
        .prefix = prefix
    };
}

uint32_t ipAddressToInt(IPAddress ip) {
    return (ip.octets[0] << 24) | (ip.octets[1] << 16) | (ip.octets[2] << 8) | ip.octets[3];
}

CustomerNode* printCustomerIPAddresses(IPAddress ip, int prefixLength, int numCustomers, int numAddressesPerCustomer) {
    uint32_t baseAddress = ipAddressToInt(ip);
    CustomerNode* head = NULL;
    CustomerNode* tail = NULL;

    for (int i = 0; i < numCustomers; i++) {
        IPAddress customerIP = intToIPAddress(baseAddress, prefixLength);

        IPAddress endIP = calculateEndIPAddress(customerIP, numAddressesPerCustomer);
        CustomerNode* node = createCustomerNode(customerIP, endIP);

        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        baseAddress += numAddressesPerCustomer;
    }

    return head;
}


int main() {
    IPAddress userIP;
    printf("Enter IP address and prefix: ");
    scanf("%hhu.%hhu.%hhu.%hhu/%d", &userIP.octets[0], &userIP.octets[1], &userIP.octets[2], &userIP.octets[3], &userIP.prefix);
    int numAddresses = calculateTotalHosts(userIP.prefix);
    printIPAddress(userIP);

    int numGroups;
    printf("Enter the number of groups: ");
    scanf("%d", &numGroups);

    printf("\nAddress Aggregations:\n");
    for (int i = 0; i < numGroups; i++) {
        GroupInfo groupInfo = getUserInputForGroup(i + 1);

        int totalAddressesOfGroup = groupInfo.numCustomers * groupInfo.numAddressesPerCustomer;
        int newPrefix = IP_BITS - getNextPowerOfTwo(totalAddressesOfGroup);

        IPAddress newIP = calculateEndIPAddress(userIP, totalAddressesOfGroup);
        userIP.prefix = newPrefix;

        printf("For Group %d:\n", i + 1);
        printAddressAggregation(userIP, newIP);

        CustomerNode* head = printCustomerIPAddresses(userIP, newPrefix, groupInfo.numCustomers, groupInfo.numAddressesPerCustomer);
        printFirstAndLast(head);
        printf("====================================\n");

        userIP = getNextIPAddress(newIP);
    }

    return 0;
}
