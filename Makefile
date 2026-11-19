.PHONY: doctor check-targets

doctor:
	python3 scripts/toolchain_doctor.py

check-targets: doctor
	@echo "AION OS 1.2: both bare-metal targets available"
