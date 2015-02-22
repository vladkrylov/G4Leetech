import re

s = "B_field_csan_Target=2mm_.*"
print re.escape(s)
p = re.compile(s)
test_strings = ["pattern1", "pattern2-", "pattern3", "pattern(4)", "pattern5", "pattern6", "pattern7", "pattern8", 
		"B_field_csan_Target=2mm_5.0 MeV_B=596 G"]
for ts in test_strings:
    if p.match(ts):   
        print ts
        