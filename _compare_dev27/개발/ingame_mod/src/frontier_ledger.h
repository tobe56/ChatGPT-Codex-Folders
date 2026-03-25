#ifndef FRONTIER_LEDGER_H
#define FRONTIER_LEDGER_H

typedef struct FrontierLedgerEntry {
    const char *label;
    const char *detail;
} FrontierLedgerEntry;

typedef struct FrontierLedgerSection {
    const char *title;
    int line_count;
    const char *const *lines;
} FrontierLedgerSection;

typedef struct FrontierLedgerPage {
    const char *page_id;
    const char *title;
    const char *summary;
    int entry_count;
    const FrontierLedgerEntry *entries;
} FrontierLedgerPage;

typedef struct FrontierLedgerDataset {
    const char *dataset_id;
    const char *display_name;
    int compact_section_count;
    const FrontierLedgerSection *compact_sections;
    int page_count;
    const FrontierLedgerPage *pages;
} FrontierLedgerDataset;

typedef struct FrontierVector3 {
    float x;
    float y;
    float z;
} FrontierVector3;

#endif
