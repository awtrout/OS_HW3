#include <stdio.h>
#include <stdint.h>

// Define a constant for the number of pages and frames
#define NUM_PAGES 256
#define NUM_FRAMES 256

// Page frame table data structure
struct PageFrameTable
{
  uint8_t page_table[NUM_PAGES];
};

// Initialize the page frame table
void
initialize_page_frame_table (struct PageFrameTable *page_frame_table)
{
  for (int i = 0; i < NUM_PAGES; i++)
	{
	  page_frame_table->page_table[i] = -1;	// Initialize all pages as not present in memory
	}
}

// Map a logical page number to a physical frame number
void
map_page_to_frame (struct PageFrameTable *page_frame_table,
				   uint8_t page_number, uint8_t frame_number)
{
  page_frame_table->page_table[page_number] = frame_number;
}

// Translate a logical address to a physical address
void
translate_logical_address (struct PageFrameTable *page_frame_table,
						   uint16_t logical_address)
{
  uint8_t page_number = (logical_address >> 8) & 0xFF;	// Extract page number
  uint8_t offset = logical_address & 0xFF;	// Extract offset

  // Check if the page is present in memory
  if (page_frame_table->page_table[page_number] != -1)
	{
	  uint8_t frame_number = page_frame_table->page_table[page_number];
	  uint16_t physical_address = (frame_number << 8) | offset;
	  printf
		("Logical Address: 0x%04X => Page Number: 0x%02X, Offset: 0x%02X => Physical Address: 0x%04X\n",
		 logical_address, page_number, offset, physical_address);
	}
  else
	{
	  printf ("Page fault occurred for logical address: 0x%04X\n",
			  logical_address);
	}
}

int
main ()
{
  struct PageFrameTable page_frame_table;
  initialize_page_frame_table (&page_frame_table);

  // Mapping some logical pages to frames
  map_page_to_frame (&page_frame_table, 0x0D, 0x1F);
  map_page_to_frame (&page_frame_table, 0x2B, 0x4E);
  map_page_to_frame (&page_frame_table, 0x15, 0x7A);

  // Sample logical addresses
  uint16_t logical_addresses[] = { 0x3A7F, 0xABCD, 0x5678 };
  int num_addresses =
	sizeof (logical_addresses) / sizeof (logical_addresses[0]);

  // Translate logical addresses to physical addresses
  for (int i = 0; i < num_addresses; i++)
	{
	  translate_logical_address (&page_frame_table, logical_addresses[i]);
	}

  return 0;
}
