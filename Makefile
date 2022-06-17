.PHONY: clean All

All:
	@echo "----------Building project:[ CIFE - GTK-Release wx3.1 64bit static ]----------"
	@cd "CIFE" && "$(MAKE)" -f  "CIFE.mk"
clean:
	@echo "----------Cleaning project:[ CIFE - GTK-Release wx3.1 64bit static ]----------"
	@cd "CIFE" && "$(MAKE)" -f  "CIFE.mk" clean
