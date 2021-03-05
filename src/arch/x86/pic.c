/**
 * Copyright (C) 2021 Romain CADILHAC
 *
 * This file is part of Silicium OS.
 *
 * Silicium OS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Silicium OS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Silicium OS.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <arch/x86/io.h>
#include <arch/x86/pic.h>

/**
 * @brief Active la gestion de toutes les IRQs
 */
void pic_enable(void)
{
	outbp(PIC_MASTER_DATA, 0x00);
	outbp(PIC_SLAVE_DATA, 0x00);
}

/**
 * @brief Désactive la gestion de toutes les IRQs
 */
void pic_disable(void)
{
	outbp(PIC_MASTER_DATA, 0xFF);
	outbp(PIC_SLAVE_DATA, 0xFF);
}

/**
 * @brief Permet d'initialiser le PIC mais surtout de remapper les IRQs à un autre
 * endroit, par défaut IRQ_BASE = 0 pour le PIC mais les 32 premières interruptions
 * sont réservées pour la gestion des exceptionsn, c'est pourquoi on déplace les
 * IRQs après
 */
void pic_remap_irq(void)
{
	outbp(PIC_MASTER_CMD, PIC_ICW1_NEED_ICW4 | PIC_ICW1_INIT_REQUIRED);
	outbp(PIC_SLAVE_CMD, PIC_ICW1_NEED_ICW4 | PIC_ICW1_INIT_REQUIRED);

	outbp(PIC_MASTER_CMD, IRQ_BASE);
	outbp(PIC_SLAVE_CMD, IRQ_BASE + IRQ_PER_PIC);

	outbp(PIC_MASTER_CMD, 4);		// Informer au PIC maître que un PIC esclave est relié à l'IRQ 2
	outbp(PIC_SLAVE_CMD, 2);		// Informer au PIC esclave qu'il est relié à l'IRQ 2 du PIC maître

	// Initialise du PIC pour les processeur 8086 et ses descendants (i386, i486, i586...)
	outbp(PIC_MASTER_CMD, PIC_ICW4_8086);
	outbp(PIC_SLAVE_CMD, PIC_ICW4_8086);
}

/**
 * @brief Permet d'envoyer un End-Of-Interruption qui permet au PIC d'envoyer
 * à nouveau l'interruption concerné car le PIC n'envoye plus l'interruption
 * tant que le EOI de l'interruption concernée n'a pas été reçu
 * 
 * @param irq L'IRQ dont on veut envoyer un EOI
 */
void pic_send_eoi(const uint_t irq)
{
	assert(irq < PIC_TOTAL_IRQ);

	if(irq >= IRQ_PER_PIC)
		outbp(PIC_SLAVE_DATA, PIC_EOI);
	outbp(PIC_MASTER_DATA, PIC_EOI);
}

/**
 * @brief Active l'IRQ spécifié
 * 
 * @param irq L'IRQ à activer
 */
void pic_enable_irq(const uint_t irq)
{
	assert(irq < PIC_TOTAL_IRQ);

	const uint8_t pic = (irq >= IRQ_PER_PIC) ? PIC_SLAVE_DATA : PIC_MASTER_DATA;
    const uint8_t mask = inb(pic);
    outbp(pic, mask & ~(1 << (irq % 8)));
}

/**
 * @brief Désactive l'IRQ spécifié
 * 
 * @param irq L'IRQ à activer
 */
void pic_disable_irq(const uint_t irq)
{
	const uint8_t pic = (irq >= IRQ_PER_PIC) ? PIC_SLAVE_DATA : PIC_MASTER_DATA;
    const uint8_t mask = inb(pic);
    outbp(pic, mask | (1 << (irq % 8)));
}