.PHONY: clean All

All:
	@echo "----------Building project:[ CIFE - Debug - GTK wx3.1 static ]----------"
	@cd "CIFE" && "$(MAKE)" -f  "CIFE.mk"
clean:
	@echo "----------Cleaning project:[ CIFE - Debug - GTK wx3.1 static ]----------"
	@cd "CIFE" && "$(MAKE)" -f  "CIFE.mk" clean
