typedef struct {
	int(*payloadFunction)(int, int);
	int delay;
} PAYLOAD;

extern const PAYLOAD payloads[];
extern const size_t nPayloads;